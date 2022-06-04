#pragma once

#include <limits>
#include <vector>
#include <numbers>
#include <ctime>
#include <chrono>
#include <iostream>

#include <glm/glm.hpp>


namespace rtx {
#define BENCHMARK(FUNCTION_CALL)                        \
    {                                                   \
        auto before = std::chrono::system_clock::now(); \
        FUNCTION_CALL;                                  \
        auto after = std::chrono::system_clock::now();  \
        std::cout << (after - before) << std::endl;     \
    }


using Color = glm::vec3;
using Point = glm::vec3;
using Vector = glm::vec3;
using Albedo = glm::vec4;

struct Ray {
    Point origin;
    Vector direction;

public:
    Ray(const Point& origin, const Vector& direction);

    Point at(float t) const;
};


class Render;
class Camera {
public:
    Camera(const Point& position, float fov, size_t width, size_t height);

    Ray emit_ray(const size_t h_pos, const size_t w_pos) const;

    size_t width() const { return _width; }
    size_t height() const { return _height; }

private:
    Point _position;
    float _field_of_view;
    size_t _width, _height;
};


struct Light {
    Point position;
    float intensity;
public:
    Light(const Point& position, float intensity);

    float diffuse(const Point& hit_point, const Vector& normal) const;
    float specular(const Point& hit_point, const Vector& normal,
                   const Vector& ray_direction, float exp) const;
};

struct Material {
    Albedo albedo;
    Color diffuse_color;
    float specular_exponent;
    float refractive_index;

public:
    Material() = default;
    Material(const Albedo& albedo, const Color& color, float exponent, float ref);
};

Vector reflect(const Vector& input, const Vector& normal);
Vector refract(const Vector& input, const Vector& normal, float refract);

struct Sphere;
struct Hit {
    float t = std::numeric_limits<float>::min();
    Sphere const* sphere = nullptr;
    Point point;
    Vector normal;

public:
    bool is_valid() const;
};

struct Object {
public:
    Object() = default;
    virtual ~Object() = default;

    virtual Hit ray_intersect(const Ray& ray) const = 0;
};

struct Sphere final : public Object {
    Point center;
    Material material;
    float radius;

public:
    Sphere(const Point& center, const Material& material, float radius);

    Hit ray_intersect(const Ray& ray) const override;
};

struct Scene final : public Object {
    std::vector<Sphere> spheres;
    std::vector<Light> lights;

public:
    Scene(const std::vector<Sphere>& spheres,
          const std::vector<Light>& lights);

    Hit ray_intersect(const Ray& ray) const override;
};


using Framebuffer = std::vector<Color>;
class Render {
    static constexpr float DELTA = 0.0001;

public:
    Render(const Scene& scene, const Color& background, const Camera& camera);

    void render();

    size_t width() const { return _camera.width(); }
    size_t height() const { return _camera.height(); }
    const Framebuffer& buffer() const { return _framebuffer; }

private:
    Color cast_ray(const Ray& ray, size_t depth = 0) const;
    size_t eval_index(const size_t h_pos, const size_t w_pos) const;

    Ray reflection_ray(const Ray& primary, const Hit& last_hit) const;
    Ray refraction_ray(const Ray& primary, const Hit& last_hit) const;
    bool is_shaded(const Light& light, const Hit& last_hit) const;

private:
    Scene _scene;
    Framebuffer _framebuffer;
    Color _background;
    Camera _camera;
};

}
