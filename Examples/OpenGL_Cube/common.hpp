#pragma once

#include <iostream>
#include <filesystem>

#include <GLFW/glfw3.h>

#include <UI/ui.hpp>
#include <OpenGL/camera.hpp>
#include <OpenGL/opengl_utils.hpp>
#include <Loader/opengl_converter.hpp>
#include <UI/application.hpp>
#include <UI/io.hpp>

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 960;


auto read_cube() {
    auto items = loader::Converter().read("./cube.obj");
    return items;
}

static void on_key(GLFWwindow*, int k, int s, int a, int m) {
    ui::io::IO::instance().emit(ui::KeyEvent(k, s, a, m));
}

static void on_mouse_moved(GLFWwindow*, double x, double y) {
    ui::io::IO::instance().emit(ui::MouseEvent(x, y));
}

static void on_mouse_pressed(GLFWwindow*, int b, int a, int m) {
    ui::io::IO::instance().emit(ui::MouseButtonEvent(b, a, m));
}

static void on_scrolled(GLFWwindow*, double x, double y) {
    ui::io::IO::instance().emit(ui::ScrollEvent(x, y));
}

static void on_dropped(GLFWwindow*, int count, const char** paths) {
    ui::io::IO::instance().emit(ui::DropEvent(count, paths));
}

auto create_camera() {
    opengl::Camera cam(WIDTH, HEIGHT, glm::radians(45.0), { 10, 10, 10 });
    return cam;
}

void init_io(GLFWwindow* window) {
    glfwSetKeyCallback(window, on_key);
    glfwSetCursorPosCallback(window, on_mouse_moved);
    glfwSetMouseButtonCallback(window, on_mouse_pressed);
    glfwSetScrollCallback(window, on_scrolled);
    glfwSetDropCallback(window, on_dropped);
}
