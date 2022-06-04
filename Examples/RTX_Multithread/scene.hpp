#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ui.hpp>
#include <opengl.hpp>
#include <rtx.hpp>
#include <utils.hpp>


void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

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
        rtx::Point(-(radius + 10), 0.0, 0.0),
        material,
        radius
    };
}

static constexpr size_t WIDTH = 1280;
static constexpr size_t HEIGHT = 860;

const std::vector<rtx::Sphere>& make_spheres() {
    static std::vector<rtx::Sphere> spheres {
        rtx::Sphere({0.0, -10.0, -30.0}, glass(), 5.0f),
        rtx::Sphere({-5.0, 10.0, -30.0}, ivory(), 3.0),
        rtx::Sphere({0.0, 0.0, -40.0}, mirror(), 10.0f),
        rtx::Sphere({5.0, 20.0, -45.0}, red_rubber(), 5.0f),
        //make_ground(red_rubber(), 10'000'000)
    };
    return spheres;
}

const std::vector<rtx::Light>& make_lights() {
    static std::vector<rtx::Light> lights {
        rtx::Light({10.0, 60.0, 0.0}, 1.0),
        rtx::Light({-10.0, 60.0, 0.0}, 1.1)
    };
    return lights;
}

const rtx::Camera& make_camera() {
    static rtx::Camera camera({0.0f, 0.0f, 30.0f},
                              rtx::utils::to_radians(40), WIDTH, HEIGHT);
    return camera;
}

const rtx::Scene& make_scene() {
    static rtx::Scene scene(make_spheres(), make_lights());
    return scene;
}
