#include <memory>

#include <RTX/rtx.hpp>

#include "rtx_scene_default.hpp"


namespace rtx {

Material mirror() {
    return {
        {0.0, 10.0, 0.8, 0.0},
        {1.0, 1.0, 1.0},
        1425.0,
        1.0
    };
}

Material ivory() {
    return {
        {0.6, 0.3, 0.1, 0.0},
        {0.45, 0.45, 0.35},
        50.0,
        1.0
    };
}

Material red_rubber() {
    return {
        {0.9, 0.1, 0.0, 0.0},
        Color(0.3, 0.1, 0.1),
        10.0f,
        1.0
    };
}

Material glass() {
    return {
        {0.0, 0.5, 0.1, 0.8},
        {0.6, 0.7, 0.8},
        125.0,
        1.5
    };
}

Sphere make_ground(const Material& material, float radius) {
    return Sphere{
        Point(0.0, -(radius + 10), 0.0),
        material,
        radius
    };
}

std::vector<rtx::Sphere> make_spheres() {
    return std::vector<rtx::Sphere> {
        rtx::Sphere(Point{0.0, 0.0, 0.0}, mirror(), 5.0f),
        rtx::Sphere(Point{15.0, 0.0, 0.0}, ivory(), 5.0),
        rtx::Sphere(Point{0.0, 20.0, 0.0}, red_rubber(), 3.0f),
        rtx::Sphere(Point{-15.0, 0.0, 0.0}, glass(), 5.0f),
        //make_ground(red_rubber(), 10'000'000)
    };
}

std::vector<Light> make_lights() {
    return {
        Light({10.0f, 60.0f, 0.0f}, 1.0),
        Light({10.0f, 60.0f, 0.0f}, 1.1)
    };
}

Camera make_camera() {
    return Camera (
        {0.0f, 0.0f, 40.0f},
        {0.0f, 0.0, 0.0f},
        utils::to_radians(90), WIDTH, HEIGHT);
}

Scene<Sphere> make_scene() {
    return Scene<Sphere>(std::move(make_spheres()), std::move(make_lights()));
}

}
