#pragma once

#include <limits>
#include <vector>
#include <numbers>

#include <glm/glm.hpp>


namespace rtx {
using Color = glm::vec3;
using Point = glm::vec3;
using Vector = glm::vec3;
using Albedo = glm::vec4;

static constexpr int WIDTH = 1024;
static constexpr int HEIGHT = 768;

struct Ray {
    Point origin;
    Vector direction;

public:
    Ray(const Point& origin, const Vector& direction)
        : origin(origin)
        , direction(direction)
    {}

    Point at(float t) const { return origin + t * direction; }
};

struct Light {
    Point position;
    float intensity;

public:
    Light(const Point& position, float intensity)
        : position(position)
        , intensity(intensity)
    {}
};

struct Material {
    Albedo albedo;
    Color diffuse_color;
    float specular_exponent;
    float refractive_index;

public:
    Material() = default;
    Material(const Albedo& albedo, const Color& color, float exponent, float ref)
        : albedo(albedo)
        , diffuse_color(color)
        , specular_exponent(exponent)
        , refractive_index(ref)
    {}
};

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

class Sphere;
struct Hit {
    float t = std::numeric_limits<float>::min();
    Sphere const* sphere = nullptr;
    Point point;
    Vector normal;

public:
    bool is_valid() const {
        return !std::fabs(t - std::numeric_limits<float>::min() <
                          std::numeric_limits<float>::epsilon());
    }
};


struct Sphere {
    Point center;
    Material material;
    float radius;

public:
    Sphere(const Point& center, const Material& material, float radius)
        : center(center)
        , material(material)
        , radius(radius)
    {}

    Hit ray_intersect(const Ray& ray) const {
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
};


Hit scene_intersect(const Ray& ray, const std::vector<Sphere>& spheres) {
    static constexpr float LIMIT = 1000.0;
    float spheres_dist = std::numeric_limits<float>::max();

    Hit out_hit;
    for (const Sphere& sphere: spheres) {
        Hit hit = sphere.ray_intersect(ray);
        if (hit.is_valid() && hit.t < spheres_dist) {
            spheres_dist = hit.t;
            out_hit = hit;
        }
    }

    return spheres_dist < LIMIT ? out_hit : Hit();
}


Color cast_ray(const Ray& ray,
               const std::vector<Sphere>& spheres,
               const std::vector<Light>& lights,
               size_t depth = 0) {
    static constexpr Color BACKGROUND(0.2, 0.85, 0.6);

    Hit last_hit = scene_intersect(ray, spheres);
    if (depth > 4 || !last_hit.is_valid()) { return BACKGROUND; }

    Vector reflect_direction = glm::normalize(reflect(ray.direction, last_hit.normal));
    Vector reflect_origin = glm::dot(reflect_direction, last_hit.normal) < 0.0 ?
        last_hit.point - last_hit.normal * 0.0001f :
        last_hit.point + last_hit.normal * 0.0001f;
    Color reflect_color = cast_ray(Ray(reflect_origin, reflect_direction),
                                   spheres, lights, depth + 1);

    Vector refract_dir = glm::normalize(
        refract(ray.direction, last_hit.normal,
                last_hit.sphere->material.refractive_index));
    Vector refract_orig = glm::dot(refract_dir, last_hit.normal) < 0.0f ?
        last_hit.point - last_hit.normal * 0.0001f :
        last_hit.point + last_hit.normal * 0.0001f;
    Color refract_color = cast_ray(Ray(refract_orig, refract_dir),
                                   spheres, lights, depth + 1);

    float diffuse_light_intensity = 0, specular_light_intensity = 0;
    const auto& material = last_hit.sphere->material;
    const auto& normal = last_hit.normal;
    const auto& hit_point = last_hit.point;
    float exponent = material.specular_exponent;
    for (const Light& light : lights) {
        Vector light_direction = glm::normalize(light.position - last_hit.point);
        float light_distance = glm::length(light.position - hit_point);

        Point shadow_origin = glm::dot(light_direction, normal) < 0.0f ?
              hit_point - normal * 0.0001f : hit_point + normal * 0.0001f;
        Ray shadow_ray(shadow_origin, light_direction);
        Hit shadow_hit = scene_intersect(shadow_ray, spheres);
        if (shadow_hit.is_valid() && glm::length(shadow_hit.point - shadow_origin) < light_distance) {
            continue;
        }

        diffuse_light_intensity += light.intensity *
            std::max(0.0f, glm::dot(light_direction, normal));
        specular_light_intensity +=
            powf(std::max(0.0f, glm::dot(reflect(light_direction, normal), ray.direction)), exponent) * light.intensity;
    }

    auto& albedo = material.albedo;
    Color total_color = material.diffuse_color *
                        diffuse_light_intensity * albedo.x +
                        Color(1.0, 1.0, 1.0) * specular_light_intensity * albedo.y +
                        reflect_color * albedo.z +
                        refract_color * albedo.w;
    return total_color;
}

Ray emit_ray(size_t i, size_t j) {
    static constexpr int fov = std::numbers::pi_v<float> / 2.0;

    float x = (2 * (i + 0.5) / (float)WIDTH - 1) * tan(fov / 2.) * WIDTH / (float)HEIGHT;
    float y = (2 * (j + 0.5) / (float)HEIGHT - 1) * tan(fov / 2.);
    Vector direction = glm::normalize(Vector(x, y, -1));
    return Ray{Point(0.0, 0.0, 0.0), direction};
}

size_t eval_index(const size_t h_pos, const size_t w_pos) {
    return w_pos + h_pos * WIDTH;
}

void render(std::vector<Color>& framebuffer,
            const std::vector<Sphere>& spheres,
            const std::vector<Light>& lights) {
    static constexpr int fov = std::numbers::pi_v<float> / 2.;

    for (size_t i = 0; i < HEIGHT; ++i) {
        for (size_t j = 0; j < WIDTH; ++j) {
            const size_t index = eval_index(i, j);
            auto pixel = cast_ray(emit_ray(i, j), spheres, lights);
            framebuffer[index] = pixel;
        }
    }
}

}
