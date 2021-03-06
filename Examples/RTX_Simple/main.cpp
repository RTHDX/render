#include <limits>
#include <cmath>
#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <UI/ui.hpp>
#include <OpenGL/opengl_proc.hpp>

#include "rtx.hpp"

void process_input(GLFWwindow* window);

#define BENCHMARK(FUNCTION_CALL)                        \
    {                                                   \
        auto before = std::chrono::system_clock::now(); \
        FUNCTION_CALL;                                  \
        auto after = std::chrono::system_clock::now();  \
        std::cout << (after - before) << std::endl;     \
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
    return rtx::Sphere {
        rtx::Point(-(radius + 10), 0.0, 0.0),
        material,
        radius
    };
}

int main() {
    if (!ui::init_glfw_lite()) { return EXIT_FAILURE; }
    auto* window = ui::create_window(rtx::WIDTH, rtx::HEIGHT, "RTX Simple");
    //glfwSetFramebufferSizeCallback(window, opengl::framebuffer_size_callback);

    opengl::Context::instance().initialize();
    opengl::Context::instance().dump();

    std::vector<rtx::Color> framebuffer(rtx::WIDTH * rtx::HEIGHT);

    std::vector<rtx::Sphere> scene {
        rtx::Sphere({0.0, -10.0, -30.0}, glass(), 5.0f),
        rtx::Sphere({-5.0, 10.0, -30.0}, ivory(), 3.0),
        rtx::Sphere({0.0, 0.0, -40.0}, mirror(), 10.0f),
        rtx::Sphere({5.0, 20.0, -45.0}, red_rubber(), 5.0f),
        //make_ground(red_rubber(), 10'000'000)
    };
    std::vector<rtx::Light> lights {
        rtx::Light({10.0, 60.0, 0.0}, 1.0),
        rtx::Light({-10.0, 60.0, 0.0}, 1.1)
    };

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        BENCHMARK(rtx::render(framebuffer, scene, lights));
        glDrawPixels(rtx::WIDTH, rtx::HEIGHT, GL_RGB, GL_FLOAT,
                     glm::value_ptr(*framebuffer.data()));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
