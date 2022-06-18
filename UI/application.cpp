#include <iostream>

#include <GLFW/glfw3.h>

#include "ui.hpp"
#include "application.hpp"
#include "io.hpp"


namespace ui {

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


Application& Application::instance() {
    static Application self;
    return self;
}

Application::Application() : Publisher() {
    init_glfw(4, 6);
    _window = create_window(860, 680, "");

    glfwSetKeyCallback(_window, on_key);
    glfwSetCursorPosCallback(_window, on_mouse_moved);
    glfwSetMouseButtonCallback(_window, on_mouse_pressed);
    glfwSetScrollCallback(_window, on_scrolled);
    glfwSetDropCallback(_window, on_dropped);
}

void Application::title(const std::string_view title) {
    if (_window) {
        glfwSetWindowTitle(_window, title.data());
    } else {
        std::cerr << "[Application] Window haven't been created" << std::endl;
    }
}

GLFWwindow* Application::window() const {
    return _window;
}

}
