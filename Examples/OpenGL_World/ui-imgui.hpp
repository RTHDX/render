#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "item.hpp"

namespace ui::imgui {

ImGuiIO& init_imgui(GLFWwindow* window, const char* glsl_version);
void cleanup(GLFWwindow* window);
void pre_process();
void render_imgui();

void show_main_widget(Scene& scene);

}
