#pragma once

#include <thread>
#include <iostream>

#include "rtx_internal.hpp"
#include "camera.hpp"
#include "scene.hpp"

namespace rtx {
using Framebuffer = std::vector<Color>;

template<Traceable T> class BaseRender {
public:
    BaseRender(Scene<T>&& scene, Camera&& camera,
               const Color& background = {0.5, 0.5, 0.5});
    virtual ~BaseRender() = default;

    virtual void render() = 0;

    size_t width() const { return _camera.width(); }
    size_t height() const { return _camera.height(); }
    const Framebuffer& buffer() const { return _framebuffer; }
    Camera& camera() { return _camera; }
    const Scene<T>& scene() const { return _scene; }

protected:
    Color cast_ray(const Ray& ray, size_t depth = 0) const;
    size_t eval_index(const size_t h_pos, const size_t w_pos) const;
    Ray reflection_ray(const Ray& primary, const Hit& last_hit) const;
    Ray refraction_ray(const Ray& primary, const Hit& last_hit) const;
    bool is_shaded(const Light& light, const Hit& last_hit) const;

protected:
    static constexpr float DELTA = 0.0001;

    Scene<T> _scene;
    Framebuffer _framebuffer;
    Color _background;
    Camera _camera;
    CameraListener _cam_handler;
};


template <Traceable T>
BaseRender<T>::BaseRender(Scene<T>&& scene, Camera&& camera,
                          const Color& back)
    : _scene(std::move(scene))
    , _framebuffer(camera.width()* camera.height())
    , _background(back)
    , _camera(std::move(camera))
    , _cam_handler(_camera)
{}

template <Traceable T>
Color BaseRender<T>::cast_ray(const Ray& ray, size_t depth) const {
    Hit last_hit = _scene.ray_intersect(ray);
    if (depth > 4 || !last_hit.is_valid()) { return _background; }

    Color reflect_color = cast_ray(reflection_ray(ray, last_hit), depth + 1);
    Color refract_color = cast_ray(refraction_ray(ray, last_hit), depth + 1);

    float diffuse_light_intensity = 0, specular_light_intensity = 0;
    const auto* material = last_hit.material;
    for (const Light& light : _scene.lights) {
        if (is_shaded(light, last_hit)) { continue; }
        diffuse_light_intensity += light.diffuse(last_hit.point, last_hit.normal);
        specular_light_intensity += light.specular(last_hit.point,
            last_hit.normal,
            ray.direction,
            material->specular_exponent);
    }

    auto& albedo = material->albedo;
    Color total_color =
        material->diffuse_color * diffuse_light_intensity * albedo.x +
        Color(1.0, 1.0, 1.0) * specular_light_intensity * albedo.y +
        reflect_color * albedo.z +
        refract_color * albedo.w;
    return total_color;
}

template <Traceable T>
size_t BaseRender<T>::eval_index(const size_t h_pos, const size_t w_pos) const {
    return w_pos + h_pos * width();
}

template <Traceable T>
Ray BaseRender<T>::reflection_ray(const Ray& primary, const Hit& last_hit) const {
    Vector direction = reflect(primary.direction, last_hit.normal);
    Vector origin = glm::dot(direction, last_hit.normal) < 0.0 ?
        last_hit.point - last_hit.normal * DELTA :
        last_hit.point + last_hit.normal * DELTA;
    return Ray(origin, glm::normalize(direction));
}

template <Traceable T>
Ray BaseRender<T>::refraction_ray(const Ray& primary, const Hit& last_hit) const {
    float ref_i = last_hit.material->refractive_index;
    Vector direction = refract(primary.direction, last_hit.normal, ref_i);
    Vector origin = glm::dot(direction, last_hit.normal) < 0.0f ?
        last_hit.point - last_hit.normal * DELTA :
        last_hit.point + last_hit.normal * DELTA;
    return Ray(origin, glm::normalize(direction));
}

template <Traceable T>
bool BaseRender<T>::is_shaded(const Light& light, const Hit& hit) const {
    Vector light_direction = glm::normalize(light.position - hit.point);
    float light_distance = glm::length(light.position - hit.point);

    Point shadow_origin = glm::dot(light_direction, hit.normal) < 0.0f ?
        hit.point - hit.normal * DELTA :
        hit.point + hit.normal * DELTA;

    Hit shadow_hit = _scene.ray_intersect(Ray(shadow_origin, light_direction));
    return shadow_hit.is_valid() &&
        glm::length(shadow_hit.point - shadow_origin) < light_distance;
}


template<Traceable T> class Render final : public BaseRender<T> {
public:
    using BaseRender<T>::BaseRender;
    ~Render() override = default;

    void render() override;
};

template <Traceable T>
void Render<T>::render() {
    for (size_t i = 0; i < height(); ++i) {
        for (size_t j = 0; j < width(); ++j) {
            const size_t index = eval_index(i, j);
            Ray ray = _camera.emit_ray(i, j);
            _framebuffer[index] = cast_ray(ray);
        }
    }
}


template<Traceable T> class MultiThreadRender final : public BaseRender<T> {
public:
    MultiThreadRender(Scene<T>&& s, Camera&& camera, Color back);
    void render() override;

private:
    void partial_rander(size_t w_start, size_t w_end);

private:
    std::vector<std::thread> _workers;
    size_t _step;
};

template <Traceable T>
MultiThreadRender<T>::MultiThreadRender(Scene<T>&& scene, Camera&& camera,
                                        Color background)
    : BaseRender<T>(std::move(scene), std::move(camera), std::move(background))
    , _workers(std::thread::hardware_concurrency())
    , _step(camera.width() / _workers.size())
{
    std::cout << "Threads supported: " << _workers.size() << std::endl;
    std::cout << "Step: " << _step << std::endl;
}

template <Traceable T>
void MultiThreadRender<T>::render() {
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

template <Traceable T>
void MultiThreadRender<T>::partial_rander(size_t w_start, size_t w_end) {
    for (size_t i = 0; i < height(); ++i) {
        for (size_t j = w_start; (w_end < width() ? j <= w_end : j < w_end); ++j) {
            const size_t index = eval_index(i, j);
            Ray ray = _camera.emit_ray(i, j);
            _framebuffer[index] = cast_ray(ray);
        }
    }
}

}
