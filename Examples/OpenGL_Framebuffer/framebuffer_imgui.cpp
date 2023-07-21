#include <array>
#include <vector>
#include <filesystem>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <UI/ui.hpp>
#include <UI/widgets.hpp>
#include <UI/imgui_widget_render.hpp>
#include <UI/io.hpp>

#include <OpenGL/texture.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/opengl_vertex_input.hpp>
#include <OpenGL/camera.hpp>


namespace fs = std::filesystem;


static constexpr int WIDTH = 800;
static constexpr int HEIGHT = 600;
static constexpr float M_RECT = 1.0f;
static constexpr float F_RECT = 1.0f;


using scene_input_t = opengl::vec3pos_vec2tex_t;
using fbuffer_input_t = opengl::vec3pos;

const std::vector<scene_input_t>& main_rect() {
    static std::vector<scene_input_t> data{
        {{ M_RECT,  M_RECT, 0.0f}, {1.0f, 1.0f}},
        {{ M_RECT, -M_RECT, 0.0f}, {1.0f, 0.0f}},
        {{-M_RECT, -M_RECT, 0.0f}, {0.0f, 0.0f}},

        {{-M_RECT, -M_RECT, 0.0f}, {0.0f, 0.0f}},
        {{-M_RECT,  M_RECT, 0.0f}, {0.0f, 1.0f}},
        {{ M_RECT,  M_RECT, 0.0f}, {1.0f, 1.0f}},
    };
    return data;
}

const std::vector<fbuffer_input_t>& fbuffer_rect() {
    static std::vector<fbuffer_input_t> data {
        {{ F_RECT,  F_RECT, 0.0f}},
        {{ F_RECT, -F_RECT, 0.0f}},
        {{-F_RECT, -F_RECT, 0.0f}},

        {{-F_RECT, -F_RECT, 0.0f}},
        {{-F_RECT,  F_RECT, 0.0f}},
        {{ F_RECT,  F_RECT, 0.0f}},
    };
    return data;
}

const std::vector<fbuffer_input_t>& fbuff_vertex_input() {
    static std::vector<fbuffer_input_t> out{
        {{ F_RECT,  F_RECT, 0.0f}},
        {{ F_RECT, -F_RECT, 0.0f}},
        {{-F_RECT, -F_RECT, 0.0f}},
        {{-F_RECT,  F_RECT, 0.0f}}
    };
    return out;
}

const std::vector<GLuint>& fbuff_vertex_indices() {
    static std::vector<GLuint> out {
        0, 1, 3,
        1, 2, 3
    };
    return out;
}


std::vector<ui::widget_sptr_t> create_ui() {
    auto rect_entity = ui::CanvasEntity::create<fbuffer_input_t>(
        fs::path("./vec3pos_MVP.vert"),
        fs::path("./fixed_color.frag"),
        fbuff_vertex_input(),
        fbuff_vertex_indices()
    );
    rect_entity->model(glm::scale(glm::mat4(1.0), {0.1, 0.1, 1.0}));
    auto canvas = ui::Canvas::create({80.0, 100.0},
                                     "opengl_canvas",
                                     {rect_entity});
    canvas->background(glm::vec4{0.8, 0.8, 0.8, 0.2});
    auto window = ui::Window::create(
        {2, 2},
        {96, 96},
        "Fbuff container",
        ImGuiWindowFlags_NoResize    |
        ImGuiWindowFlags_NoMove      |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse,
        {canvas}
    );
    return {window};
}


int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }

    auto* win = ui::create_window(WIDTH, HEIGHT,
        "Framebuffer rendering to imgui window"
    );
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background(glm::vec4{0.8, 1.0, 0.0, 1.0 });
    ui::io::IO::instance().bind(win);

    auto ui_components = create_ui();

    if (!ui::init_imgui_opengl3_glfw(win)) {
        return EXIT_FAILURE;
    }

    ui::ImGuiWidgetRender ui_render;
    glm::mat4 one(1.0);
    glm::mat4 fbuff_scale = glm::scale(one, {0.1, 0.1, 0.0});
    while (!glfwWindowShouldClose(win)) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        glfwPollEvents();
        opengl::Context::instance().draw_background();

        for (const auto& widget : ui_components) {
            widget->accept(ui_render);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(win);
    }

    glfwDestroyWindow(win);
    glfwTerminate();
    return EXIT_SUCCESS;
}