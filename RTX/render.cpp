#include <iostream>

#include "render.hpp"


namespace rtx {

BaseRender::BaseRender(const Scene& scene, const Color& back, const Camera& camera)
    : _scene(scene)
    , _framebuffer(camera.width()* camera.height())
    , _background(back)
    , _camera(camera)
    , _cam_handler(_camera)
{}

Color BaseRender::cast_ray(const Ray& ray, size_t depth) const {
    Hit last_hit = _scene.ray_intersect(ray);
    if (depth > 4 || !last_hit.is_valid()) { return _background; }

    Color reflect_color = cast_ray(reflection_ray(ray, last_hit), depth + 1);
    Color refract_color = cast_ray(refraction_ray(ray, last_hit), depth + 1);

    float diffuse_light_intensity = 0, specular_light_intensity = 0;
    const auto& material = last_hit.sphere->material;
    for (const Light& light : _scene.lights) {
        if (is_shaded(light, last_hit)) { continue; }
        diffuse_light_intensity += light.diffuse(last_hit.point, last_hit.normal);
        specular_light_intensity += light.specular(last_hit.point,
            last_hit.normal,
            ray.direction,
            material.specular_exponent);
    }

    auto& albedo = material.albedo;
    Color total_color =
        material.diffuse_color * diffuse_light_intensity * albedo.x +
        Color(1.0, 1.0, 1.0) * specular_light_intensity * albedo.y +
        reflect_color * albedo.z +
        refract_color * albedo.w;
    return total_color;
}

size_t BaseRender::eval_index(const size_t h_pos, const size_t w_pos) const {
    return w_pos + h_pos * width();
}

Ray BaseRender::reflection_ray(const Ray& primary, const Hit& last_hit) const {
    Vector direction = reflect(primary.direction, last_hit.normal);
    Vector origin = glm::dot(direction, last_hit.normal) < 0.0 ?
        last_hit.point - last_hit.normal * DELTA :
        last_hit.point + last_hit.normal * DELTA;
    return Ray(origin, glm::normalize(direction));
}

Ray BaseRender::refraction_ray(const Ray& primary, const Hit& last_hit) const {
    float ref_i = last_hit.sphere->material.refractive_index;
    Vector direction = refract(primary.direction, last_hit.normal, ref_i);
    Vector origin = glm::dot(direction, last_hit.normal) < 0.0f ?
        last_hit.point - last_hit.normal * DELTA :
        last_hit.point + last_hit.normal * DELTA;
    return Ray(origin, glm::normalize(direction));
}

bool BaseRender::is_shaded(const Light& light, const Hit& hit) const {
    Vector light_direction = glm::normalize(light.position - hit.point);
    float light_distance = glm::length(light.position - hit.point);

    Point shadow_origin = glm::dot(light_direction, hit.normal) < 0.0f ?
        hit.point - hit.normal * DELTA :
        hit.point + hit.normal * DELTA;

    Hit shadow_hit = _scene.ray_intersect(Ray(shadow_origin, light_direction));
    return shadow_hit.is_valid() &&
        glm::length(shadow_hit.point - shadow_origin) < light_distance;
}


Render::Render(const Scene& scene, const Color& background, const Camera& camera)
    : BaseRender(scene, background, camera)
{}

void Render::render() {
    for (size_t i = 0; i < height(); ++i) {
        for (size_t j = 0; j < width(); ++j) {
            const size_t index = eval_index(i, j);
            Ray ray = _camera.emit_ray(i, j);
            _framebuffer[index] = cast_ray(ray);
        }
    }
}


MultiThreadRender::MultiThreadRender(const Scene& scene, const Color& background,
                                     const Camera& camera)
    : BaseRender(scene, background, camera)
    , _workers(std::thread::hardware_concurrency())
    , _step(camera.width() / _workers.size())
{
    std::cout << "Threads supported: " << _workers.size() << std::endl;
    std::cout << "Step: " << _step << std::endl;
}

void MultiThreadRender::render() {
    size_t current = 0;
    for (size_t i = 0; i < _workers.size(); ++i) {
        size_t start = current;
        size_t end = (start + _step) <= width() ? start + _step : width();
        _workers[i] = std::thread(&MultiThreadRender::partial_rander,
                                  this, start, end);
        current = end + 1;
    }

    for (std::thread& worker : _workers) {
        worker.join();
    }
}

void MultiThreadRender::partial_rander(size_t w_start, size_t w_end) {
    for (size_t i = 0; i < height(); ++i) {
        for (size_t j = w_start; (w_end < width() ? j <= w_end : j < w_end); ++j) {
            const size_t index = eval_index(i, j);
            Ray ray = _camera.emit_ray(i, j);
            _framebuffer[index] = cast_ray(ray);
        }
    }
}

}
