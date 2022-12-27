#pragma once

#include <limits>
#include <vector>
#include <numbers>

#include <glm/glm.hpp>


namespace rtx {
using color_t = glm::vec3;
using point_t = glm::vec3;
using vector_t = glm::vec3;
using albedo_t = glm::vec4;

static constexpr int WIDTH = 1024;
static constexpr int HEIGHT = 768;

struct ray_t {
    point_t origin;
    vector_t direction;

public:
    ray_t(const point_t& origin, const vector_t& direction)
        : origin(origin)
        , direction(direction)
    {}

    point_t at(float t) const { return origin + t * direction; }
};

struct light_t {
    point_t position;
    float intensity;

public:
    light_t(const point_t& position, float intensity)
        : position(position)
        , intensity(intensity)
    {}
};

struct material_t {
    albedo_t albedo;
    color_t diffuse_color;
    float specular_exponent;
    float refractive_index;

public:
    material_t() = default;
    material_t(const albedo_t& albedo, const color_t& color, float exponent, float ref)
        : albedo(albedo)
        , diffuse_color(color)
        , specular_exponent(exponent)
        , refractive_index(ref)
    {}
};

vector_t reflect(const vector_t& input, const vector_t& normal) {
    return input - normal * 2.0f * glm::dot(input, normal);
}

vector_t refract(const vector_t& input, const vector_t& normal, float refract) {
    float cosi = std::max(-1.0f, std::min(1.0f, glm::dot(input, normal)));
    float etai = 1.0f, etat = refract;
    vector_t n = normal;
    if (cosi < 0.0) {
        cosi = -cosi;
        std::swap(etai, etat);
        n = -normal;
    }
    float eta = etai / etat;
    float k = 1.0 - eta * eta * (1 - cosi * cosi);
    return k < 0.0 ? vector_t(0.0, 0.0, 0.0) :
        input * eta + n * (eta * cosi - sqrtf(k));
}

class Sphere;
struct hit_t {
    float t = std::numeric_limits<float>::min();
    Sphere const* sphere = nullptr;
    point_t point;
    vector_t normal;

public:
    bool is_valid() const {
        return !std::fabs(t - std::numeric_limits<float>::min() <
                          std::numeric_limits<float>::epsilon());
    }
};


struct Sphere {
    point_t center;
    material_t material;
    float radius;

public:
    Sphere(const point_t& center, const material_t& material, float radius)
        : center(center)
        , material(material)
        , radius(radius)
    {}

    hit_t ray_intersect(const ray_t& ray) const {
        vector_t L = center - ray.origin;
        float tca = glm::dot(L, ray.direction);
        float d2 = glm::dot(L, L) - tca * tca;
        if (d2 > radius * radius) return hit_t();
        float thc = sqrtf(radius * radius - d2);

        hit_t hit;
        hit.t = tca - thc;
        float t1 = tca + thc;
        if (hit.t < 0) hit.t = t1;
        if (hit.t < 0) return hit_t();
        hit.sphere = this;
        hit.point = ray.at(hit.t);
        hit.normal = glm::normalize(hit.point - center);
        return hit;
    }
};


hit_t scene_intersect(const ray_t& ray, const std::vector<Sphere>& spheres) {
    static constexpr float LIMIT = 1000.0;
    float spheres_dist = std::numeric_limits<float>::max();

    hit_t out_hit;
    for (const Sphere& sphere: spheres) {
        hit_t hit = sphere.ray_intersect(ray);
        if (hit.is_valid() && hit.t < spheres_dist) {
            spheres_dist = hit.t;
            out_hit = hit;
        }
    }

    return spheres_dist < LIMIT ? out_hit : hit_t();
}


color_t cast_ray(const ray_t& ray,
               const std::vector<Sphere>& spheres,
               const std::vector<light_t>& lights,
               size_t depth = 0) {
    static constexpr color_t BACKGROUND(0.2, 0.85, 0.6);

    hit_t last_hit = scene_intersect(ray, spheres);
    if (depth > 4 || !last_hit.is_valid()) { return BACKGROUND; }

    vector_t reflect_direction = glm::normalize(reflect(ray.direction, last_hit.normal));
    vector_t reflect_origin = glm::dot(reflect_direction, last_hit.normal) < 0.0 ?
        last_hit.point - last_hit.normal * 0.0001f :
        last_hit.point + last_hit.normal * 0.0001f;
    color_t reflect_color = cast_ray(ray_t(reflect_origin, reflect_direction),
                                   spheres, lights, depth + 1);

    vector_t refract_dir = glm::normalize(
        refract(ray.direction, last_hit.normal,
                last_hit.sphere->material.refractive_index));
    vector_t refract_orig = glm::dot(refract_dir, last_hit.normal) < 0.0f ?
        last_hit.point - last_hit.normal * 0.0001f :
        last_hit.point + last_hit.normal * 0.0001f;
    color_t refract_color = cast_ray(ray_t(refract_orig, refract_dir),
                                   spheres, lights, depth + 1);

    float diffuse_light_intensity = 0, specular_light_intensity = 0;
    const auto& material = last_hit.sphere->material;
    const auto& normal = last_hit.normal;
    const auto& hit_point = last_hit.point;
    float exponent = material.specular_exponent;
    for (const light_t& light : lights) {
        vector_t light_direction = glm::normalize(light.position - last_hit.point);
        float light_distance = glm::length(light.position - hit_point);

        point_t shadow_origin = glm::dot(light_direction, normal) < 0.0f ?
              hit_point - normal * 0.0001f : hit_point + normal * 0.0001f;
        ray_t shadow_ray(shadow_origin, light_direction);
        hit_t shadow_hit = scene_intersect(shadow_ray, spheres);
        if (shadow_hit.is_valid() && glm::length(shadow_hit.point - shadow_origin) < light_distance) {
            continue;
        }

        diffuse_light_intensity += light.intensity *
            std::max(0.0f, glm::dot(light_direction, normal));
        specular_light_intensity +=
            powf(std::max(0.0f, glm::dot(reflect(light_direction, normal), ray.direction)), exponent) * light.intensity;
    }

    auto& albedo = material.albedo;
    color_t total_color = material.diffuse_color *
                        diffuse_light_intensity * albedo.x +
                        color_t(1.0, 1.0, 1.0) * specular_light_intensity * albedo.y +
                        reflect_color * albedo.z +
                        refract_color * albedo.w;
    return total_color;
}

ray_t emit_ray(size_t i, size_t j) {
    static constexpr int fov = std::numbers::pi_v<float> / 2.0;

    float x = (2 * (i + 0.5) / (float)WIDTH - 1) * tan(fov / 2.) * WIDTH / (float)HEIGHT;
    float y = (2 * (j + 0.5) / (float)HEIGHT - 1) * tan(fov / 2.);
    vector_t direction = glm::normalize(vector_t{x, y, -1});
    return {{0.0, 0.0, 0.0}, direction};
}

size_t eval_index(const size_t h_pos, const size_t w_pos) {
    return w_pos + h_pos * WIDTH;
}

void render(std::vector<color_t>& framebuffer,
            const std::vector<Sphere>& spheres,
            const std::vector<light_t>& lights) {
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
