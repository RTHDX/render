#pragma once

#include <limits>
#include <vector>
#include <numbers>

#include <glm/glm.hpp>


namespace rtx {
using Color = glm::vec3;
using Point = glm::vec3;
using Vector = glm::vec3;

static constexpr int WIDTH = 1024;
static constexpr int HEIGHT = 768;

struct Ray {
    Point origin;
    Vector direction;

public:
    Point at(float t) const {
        return origin + t * direction;
    }
};

struct Material {
    Color diffuse_color;

public:
    Material() = default;
    Material(const Color& color) : diffuse_color(color) {}
};

class Sphere;
struct Hit {
    float hit_near = std::numeric_limits<float>::min();
    float hit_far = std::numeric_limits<float>::max();
    Sphere const* sphere = nullptr;
    Point point;
    Vector normal;

public:
    bool is_valid() {
        const bool is_near_to_min =
            std::fabs(hit_near - std::numeric_limits<float>::min() <
                std::numeric_limits<float>::epsilon());
        const bool is_near_to_max =
            std::fabs(hit_far - std::numeric_limits<float>::max() <
                std::numeric_limits<float>::epsilon());
        return !(is_near_to_max && is_near_to_min);
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

    bool ray_intersect(const Vector& orig, const Vector& dir, float& t0) const {
        Vector L = center - orig;
        float tca = glm::dot(L, dir);
        float d2 = glm::dot(L, L) - tca * tca;
        if (d2 > radius * radius) return false;
        float thc = sqrtf(radius * radius - d2);
        t0 = tca - thc;
        float t1 = tca + thc;
        if (t0 < 0) t0 = t1;
        if (t0 < 0) return false;
        return true;
    }
};


bool scene_intersect(const Point& orig, const Vector& dir,
                     const std::vector<Sphere>& spheres,
                     Point& hit, Vector& N, Material& material) {
    float spheres_dist = std::numeric_limits<float>::max();
    for (size_t i = 0; i < spheres.size(); i++) {
        float dist_i;
        if (spheres[i].ray_intersect(orig, dir, dist_i) && dist_i < spheres_dist) {
            spheres_dist = dist_i;
            hit = orig + dir * dist_i;
            N = glm::normalize(hit - spheres[i].center);
            material = spheres[i].material;
        }
    }
    return spheres_dist < 1000;
}


Color cast_ray(const Point& orig, const Vector& dir,
               const std::vector<Sphere>& spheres) {
    static constexpr Color BACKGROUND(0.2, 0.7, 0.8);

    Vector point, N;
    Material material;

    if (!scene_intersect(orig, dir, spheres, point, N, material)) {
        return BACKGROUND;
    }

    return material.diffuse_color;
}

Ray emit_ray(size_t i, size_t j) {
    static constexpr int fov = std::numbers::pi_v<float> / 2.;

    float x = (2 * (i + 0.5) / (float)WIDTH - 1) * tan(fov / 2.) * WIDTH / (float)HEIGHT;
    float y = -(2 * (j + 0.5) / (float)HEIGHT - 1) * tan(fov / 2.);
    Vector direction = glm::normalize(Vector(x, y, -1));
    return Ray{Point(0.0, 0.0, 0.0), direction};
}

size_t eval_index(const size_t h_pos, const size_t w_pos) {
    return w_pos + h_pos * WIDTH;
}

void render(std::vector<Color>& framebuffer,
            const std::vector<Sphere>& spheres) {
    static constexpr int fov = std::numbers::pi_v<float> / 2.;

    for (size_t i = 0; i < HEIGHT; ++i) {
        for (size_t j = 0; j < WIDTH; ++j) {
            const size_t index = eval_index(i, j);
            float x = (2 * (i + 0.5) / (float)WIDTH - 1) * tan(fov / 2.) * WIDTH / (float)HEIGHT;
            float y = -(2 * (j + 0.5) / (float)HEIGHT - 1) * tan(fov / 2.);
            Vector direction = glm::normalize(Vector(x, y, -1));
            framebuffer[index] = cast_ray(
                Point(0.0, 0.0, 0.0),
                direction,
                spheres
            );
        }
    }
}

}
