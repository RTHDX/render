#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <UI/ui.hpp>
#include <UI/io.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/camera.hpp>
#include <Loader/opengl_converter.hpp>

#include "io.hpp"
#include "item.hpp"
#include "ui-imgui.hpp"


constexpr int WIDTH = 1280;
constexpr int HEIGHT = 960;


glm::vec4 background   = {0.5, 0.8, 0.8, 1.0};
glm::vec4 ground_color = {0.5, 0.5, 0.5, 1.0};
glm::vec4 light_color  = {0.8, 0.8, 1.0, 1.0};
glm::vec3 light_pos    = {20.0, 20.0, 20.0};
namespace fs = std::filesystem;

auto create_empty_scene() {
    return Scene {
        opengl::Light({20.0, 20.0, 20.0}, {0.8, 0.8, 1.0, 1.0}),
        opengl::Camera(WIDTH, HEIGHT, glm::radians(45.0), {30, 30, 30})
    };
}

int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }

    auto* window = ui::create_window(WIDTH, HEIGHT, "world");
    ui::io::IO::instance().bind(window);
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background(background);
    auto& imgui_io = ui::imgui::init_imgui(window, "#version 460");

    GlobalListener g_listener(std::move(create_empty_scene()),
                              &ui::io::IO::instance());

    ui::imgui::Context ui_context(g_listener.scene());

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        opengl::Context::instance().draw_background();
        ui::imgui::pre_process();

        g_listener.scene().draw();
        ui_context.show_main_window();

        ui::imgui::render_imgui();
        glfwSwapBuffers(window);
    }

    ui::imgui::cleanup(window);
    return 0;
}
