#include "rtx.hpp"


namespace rtx {

Ray::Ray(const Point& origin, const Vector& direction)
    : origin(origin)
    , direction(direction)
{}

Point Ray::at(float t) const { return origin + t * direction; }


Camera::Camera(const Point& position, float fov, size_t width, size_t height)
    : _position(position)
    , _field_of_view(fov)
    , _width(width)
    , _height(height)
{}

Ray Camera::emit_ray(const size_t i, const size_t j) const {
    float x = (2 * (i + 0.5) / (float)_width - 1) * tan(_field_of_view / 2.) * _width / (float)_width;
    float y = (2 * (j + 0.5) / (float)_height - 1) * tan(_field_of_view / 2.);
    Vector direction = glm::normalize(Vector(x, y, -1));
    return Ray{_position, direction};
}


Light::Light(const Point& position, float intensity)
    : position(position)
    , intensity(intensity)
{}

float Light::diffuse(const Point& hit_point, const Vector& normal) const {
    Vector direction = glm::normalize(position - hit_point);
    return intensity *
           std::max(0.0f, glm::dot(direction, normal));
}

float Light::specular(const Point& point, const Vector& normal,
                      const Vector& ray_direction, float exp) const {
    Vector reflection = reflect(glm::normalize(position - point), normal);
    return intensity *
           powf(std::max(0.0f, glm::dot(reflection, ray_direction)), exp);
}


Material::Material(const Albedo& albedo, const Color& color, float exponent,
                   float ref)
    : albedo(albedo)
    , diffuse_color(color)
    , specular_exponent(exponent)
    , refractive_index(ref)
{}


Vector reflect(const Vector& input, const Vector& normal) {
    return input - normal * 2.0f * glm::dot(input, normal);
}

Vector refract(const Vector& input, const Vector& normal, float refract) {
    float cosi = std::max(-1.0f, std::min(1.0f, glm::dot(input, normal)));
    float etai = 1.0f, etat = refract;
    Vector n = normal;
    if (cosi < 0.0) {
        cosi = -cosi;
        std::swap(etai, etat);
        n = -normal;
    }
    float eta = etai / etat;
    float k = 1.0 - eta * eta * (1 - cosi * cosi);
    return k < 0.0 ? Vector(0.0, 0.0, 0.0) :
        input * eta + n * (eta * cosi - sqrtf(k));
}


bool Hit::is_valid() const {
    return !std::fabs(t - std::numeric_limits<float>::min() <
            std::numeric_limits<float>::epsilon());
}


Sphere::Sphere(const Point& center, const Material& material, float radius)
    : center(center)
    , material(material)
    , radius(radius)
{}

Hit Sphere::ray_intersect(const Ray& ray) const {
    Vector L = center - ray.origin;
    float tca = glm::dot(L, ray.direction);
    float d2 = glm::dot(L, L) - tca * tca;
    if (d2 > radius * radius) return Hit();
    float thc = sqrtf(radius * radius - d2);
    Hit hit;
    hit.t = tca - thc;
    float t1 = tca + thc;
    if (hit.t < 0) hit.t = t1;
    if (hit.t < 0) return Hit();
    hit.sphere = this;
    hit.point = ray.at(hit.t);
    hit.normal = glm::normalize(hit.point - center);
    return hit;
}


Scene::Scene(const std::vector<Sphere>& spheres,
             const std::vector<Light>& lights)
    : Object()
    , spheres(spheres)
    , lights(lights)
{}

Hit Scene::ray_intersect(const Ray& ray) const {
    static constexpr float LIMIT = 1000.0;
    float spheres_dist = std::numeric_limits<float>::max();

    Hit out_hit;
    for (const Sphere& sphere : spheres) {
        Hit hit = sphere.ray_intersect(ray);
        if (hit.is_valid() && hit.t < spheres_dist) {
            spheres_dist = hit.t;
            out_hit = hit;
        }
    }

    return spheres_dist < LIMIT ? out_hit : Hit();
}


Render::Render(const Scene& scene, const Color& background, const Camera& camera)
    : _scene(scene)
    , _framebuffer(camera.width()* camera.height())
    , _background(background)
    , _camera(camera)
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

Color Render::cast_ray(const Ray& ray, size_t depth) const {
    Hit last_hit = _scene.ray_intersect(ray);
    if (depth > 4 || !last_hit.is_valid()) { return _background; }

    Color reflect_color = cast_ray(reflection_ray(ray, last_hit), depth + 1);
    Color refract_color = cast_ray(refraction_ray(ray, last_hit), depth + 1);

    float diffuse_light_intensity = 0, specular_light_intensity = 0;
    const auto& material = last_hit.sphere->material;
    for (const Light& light: _scene.lights) {
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
        Color(1.0, 1.0, 1.0) * specular_light_intensity  * albedo.y +
        reflect_color                                    * albedo.z +
        refract_color                                    * albedo.w;
    return total_color;
}

size_t Render::eval_index(const size_t h_pos, const size_t w_pos) const {
    return w_pos + h_pos * width();
}

Ray Render::reflection_ray(const Ray& primary, const Hit& last_hit) const {
    Vector direction = reflect(primary.direction, last_hit.normal);
    Vector origin = glm::dot(direction, last_hit.normal) < 0.0 ?
        last_hit.point - last_hit.normal * DELTA :
        last_hit.point + last_hit.normal * DELTA;
    return Ray(origin, glm::normalize(direction));
}

Ray Render::refraction_ray(const Ray& primary, const Hit& last_hit) const {
    float ref_i = last_hit.sphere->material.refractive_index;
    Vector direction = refract(primary.direction, last_hit.normal, ref_i);
    Vector origin = glm::dot(direction, last_hit.normal) < 0.0f ?
        last_hit.point - last_hit.normal * DELTA :
        last_hit.point + last_hit.normal * DELTA;
    return Ray(origin, glm::normalize(direction));
}

bool Render::is_shaded(const Light& light, const Hit& hit) const {
    Vector light_direction = glm::normalize(light.position - hit.point);
    float light_distance = glm::length(light.position - hit.point);

    Point shadow_origin = glm::dot(light_direction, hit.normal) < 0.0f ?
                            hit.point - hit.normal * DELTA :
                            hit.point + hit.normal * DELTA;

    Hit shadow_hit = _scene.ray_intersect(Ray(shadow_origin, light_direction));
    return shadow_hit.is_valid() &&
           glm::length(shadow_hit.point - shadow_origin) < light_distance;
}

}
