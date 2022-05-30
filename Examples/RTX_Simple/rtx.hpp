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


class Sphere {
public:
    Sphere(const Point& center, const Material& material, float radius)
        : _center(center)
        , _material(material)
        , _radius(radius)
    {}

    Hit ray_intersects(const Ray& ray) const {
        Hit hit;
        Vector l = _center - ray.origin;
        float tca = glm::dot(l, ray.direction);
        float d2 = glm::dot(l, l) - tca * tca;
        if (d2 > _radius * _radius) { return hit; }
        float thc = std::sqrtf(_radius * _radius - d2);
        hit.hit_near = tca - thc;
        float t1 = tca + thc;
        if (hit.hit_near < 0) { hit.hit_near = t1; }
        if (hit.hit_near < 0) { return Hit(); }
        hit.sphere = this;
        hit.point = ray.at(hit.hit_near);
        hit.normal = glm::normalize(hit.point - _center);
        return hit;
    }

    const Material& material() const { return _material; }

private:
    Point _center;
    Material _material;
    float _radius;
};


Hit scene_intersects(const Ray& ray, const std::vector<Sphere>& spheres) {
    float spheres_dist = std::numeric_limits<float>::max();
    Hit hit;
    for (const Sphere& sphere : spheres) {
        hit = sphere.ray_intersects(ray);
        if (hit.is_valid() && hit.hit_near < spheres_dist) {
            spheres_dist = hit.hit_near;
        }
    }
    return hit;
}


Color cast_ray(const Ray& ray, const std::vector<Sphere>& spheres) {
    static constexpr Color BACKGROUND(0.2, 0.7, 0.8);

    float sphere_dist = std::numeric_limits<float>::max();
    Hit hit = scene_intersects(ray, spheres);
    if (hit.is_valid()) {
        return hit.sphere->material().diffuse_color;
    }

    return BACKGROUND;
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
    for (size_t i = 0; i < HEIGHT; ++i) {
        for (size_t j = 0; j < WIDTH; ++j) {
            const size_t index = eval_index(i, j);
            framebuffer[index] = cast_ray(emit_ray(i, j), spheres);
        }
    }
}

}
