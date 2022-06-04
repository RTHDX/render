#include "rtx_internal.hpp"

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

}
