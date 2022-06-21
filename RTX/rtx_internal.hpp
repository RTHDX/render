#pragma once

#include <limits>
#include <vector>
#include <numbers>
#include <ctime>
#include <chrono>
#include <iostream>

#include <glm/glm.hpp>


namespace rtx {

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
std::ostream& operator << (std::ostream& os, const Ray& ray);


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

struct Hit {
    float t = std::numeric_limits<float>::min();
    Material const* material = nullptr;
    Point point;
    Vector normal;

public:
    Hit() = default;
    Hit(float t, Material const* material, const Point& point, const Vector& n);

    bool is_valid() const;
};

struct Object {
public:
    Object() = default;
    virtual ~Object() = default;

    virtual Hit ray_intersect(const Ray& ray) const = 0;
};
using uObject = std::unique_ptr<Object>;
using sObject = std::shared_ptr<Object>;

struct Sphere final : public Object {
    Point center;
    Material material;
    float radius;

public:
    Sphere(const Point& center, const Material& material, float radius);

    Hit ray_intersect(const Ray& ray) const override;
};


struct Triangle final : public Object {
    Point a, b, c;
    Vector normal;
    Material material;

public:
    Triangle() = default;
    Triangle(const Point& a, const Point& b, const Point& c, const Vector& n,
             const Material& material);

    Hit ray_intersect(const Ray& ray) const override;
};


struct Mesh final : public Object {
    std::vector<Triangle> faces;

public:
    Mesh() = default;
    Mesh(const std::vector<Triangle>& triangles);

    Hit ray_intersect(const Ray& ray) const override;
};


struct Scene final : public Object {
    std::vector<sObject> objects;
    std::vector<Light> lights;

public:
    Scene() = delete;
    Scene(std::vector<sObject>&& objects,
          const std::vector<Light>& lights);

    Hit ray_intersect(const Ray& ray) const override;
};

}
