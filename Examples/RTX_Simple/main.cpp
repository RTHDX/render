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

#include <ui.hpp>
#include <opengl.hpp>

#include "rtx.hpp"

void process_input(GLFWwindow* window);

#define BENCHMARK(FUNCTION_CALL)                        \
    {                                                   \
        auto before = std::chrono::system_clock::now(); \
        FUNCTION_CALL;                                  \
        auto after = std::chrono::system_clock::now();  \
        std::cout << (after - before) << std::endl;     \
    }


int main() {
    if (!ui::init_glfw_lite()) { return EXIT_FAILURE; }
    auto* window = ui::create_window(rtx::WIDTH, rtx::HEIGHT, "RTX Simple");
    glfwSetFramebufferSizeCallback(window, opengl::framebuffer_size_callback);

    opengl::Context::instance().initialize();
    opengl::Context::instance().dump();

    std::vector<rtx::Color> framebuffer (rtx::WIDTH * rtx::HEIGHT);
    rtx::Sphere red(rtx::Point(0.0, 0.0, -30.0),
                    rtx::Material(rtx::Color(1.0, 0.0, 0.0)),
                    5.0f);

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        BENCHMARK(rtx::render(framebuffer, {red}));
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
