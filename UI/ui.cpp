#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ui.hpp"

namespace ui {

bool init_glfw(int major, int minor) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    glfwWindowHint(GLFW_SAMPLES, 4);
    return true;
}

bool init_glfw_lite() {
    auto result = glfwInit();
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    return result == GLFW_TRUE;
}

void unload_glfw() {
    glfwTerminate();
}

GLFWwindow* create_window(int width, int height, const char* title,
                          bool fullstreen) {
    GLFWwindow* window = nullptr;
    if (fullstreen) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        window = glfwCreateWindow(mode->width, mode->height, "",
                                  monitor, nullptr);
    } else {
        window = glfwCreateWindow(width, height, title, nullptr,
                                  nullptr);
    }

    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::terminate();
    }
    glfwMakeContextCurrent(window);
    return window;
}

bool init_opengl() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}

}
