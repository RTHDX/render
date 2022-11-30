#include <filesystem>

#include <GLFW/glfw3.h>

#include "io.hpp"

namespace ui::io {

static void on_key(GLFWwindow*, int k, int s, int a, int m) {
    ui::io::IO::instance().emit(KeyEvent(k, s, a, m));
}

static void on_mouse_moved(GLFWwindow*, double x, double y) {
    ui::io::IO::instance().emit(MouseEvent(x, y));
}

static void on_mouse_pressed(GLFWwindow*, int b, int a, int m) {
    ui::io::IO::instance().emit(MouseButtonEvent(b, a, m));
}

static void on_scrolled(GLFWwindow*, double x, double y) {
    ui::io::IO::instance().emit(ScrollEvent(x, y));
}

static void on_dropped(GLFWwindow*, int count, const char** paths) {
    ui::io::IO::instance().emit(DropEvent(count, paths));
}

void IO::bind(GLFWwindow* window) {
    glfwSetKeyCallback(window, on_key);
    glfwSetCursorPosCallback(window, on_mouse_moved);
    glfwSetMouseButtonCallback(window, on_mouse_pressed);
    glfwSetScrollCallback(window, on_scrolled);
    glfwSetDropCallback(window, on_dropped);
    _is_bound = true;
}

}
