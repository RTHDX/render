#pragma once

struct GLFWwindow;
namespace ui {

bool init_glfw(int major, int minor);
bool init_glfw_lite();
GLFWwindow* create_window(int width, int height, const char* title);
bool init_opengl();

}
