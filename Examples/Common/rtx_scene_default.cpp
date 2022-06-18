#include "rtx_scene_default.hpp"


namespace rtx {

rtx::Material mirror() {
    return {
        {0.0, 10.0, 0.8, 0.0},
        rtx::Color(1.0, 1.0, 1.0),
        1425.0,
        1.0
    };
}

rtx::Material ivory() {
    return {
        {0.6, 0.3, 0.1, 0.0},
        rtx::Color(0.45, 0.45, 0.35),
        50.0,
        1.0
    };
}

rtx::Material red_rubber() {
    return {
        {0.9, 0.1, 0.0, 0.0},
        rtx::Color(0.3, 0.1, 0.1),
        10.0f,
        1.0
    };
}

rtx::Material glass() {
    return {
        {0.0, 0.5, 0.1, 0.8},
        {0.6, 0.7, 0.8},
        125.0,
        1.5
    };
}

rtx::Sphere make_ground(const rtx::Material& material, float radius) {
    return rtx::Sphere{
        rtx::Point(0.0, -(radius + 10), 0.0),
        material,
        radius
    };
}

const std::vector<rtx::Sphere>& make_spheres() {
    static std::vector<rtx::Sphere> spheres{
        rtx::Sphere({0.0, 0.0, 0.0}, mirror(), 5.0f),
        rtx::Sphere({15.0, 0.0, 0.0}, ivory(), 5.0),
        rtx::Sphere({0.0, 20.0, 0.0}, red_rubber(), 3.0f),
        rtx::Sphere({-15.0, 0.0, 0.0}, glass(), 5.0f),
        //make_ground(red_rubber(), 10'000'000)
    };
    return spheres;
}

const std::vector<rtx::Light>& make_lights() {
    static std::vector<rtx::Light> lights{
        Light({10.0f, 60.0f, 0.0f}, 1.0),
        Light({10.0f, 60.0f, 0.0f}, 1.1)
    };
    return lights;
}

const Camera& make_camera() {
    static rtx::Camera camera({0.0f, 0.0f, 30.0f},
        rtx::utils::to_radians(90), WIDTH, HEIGHT);
    return camera;
}

const Scene& make_scene() {
    static rtx::Scene scene(make_spheres(), make_lights());
    return scene;
}

}
