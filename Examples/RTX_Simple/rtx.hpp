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
    Point at(float t) const { return origin + t * direction; }
};

struct Material {
    Color diffuse_color;

public:
    Material() = default;
    Material(const Color& color) : diffuse_color(color) {}
};

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


Color cast_ray(const Ray& ray, const std::vector<Sphere>& spheres) {
    static constexpr Color BACKGROUND(0.2, 0.7, 0.8);

    Hit last_hit = scene_intersect(ray, spheres);
    if (!last_hit.is_valid()) {
        return BACKGROUND;
    }

    return last_hit.sphere->material.diffuse_color;
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
            framebuffer[index] = cast_ray(emit_ray(i, j), spheres);
        }
    }
}

}
