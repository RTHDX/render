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


std::vector<ui::widget_sptr_t> create_ui() {
    return {
        ui::Window::create({2, 2}, {96, 96}, "Fbuff container",
                           ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove),
    };
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
/*
    IMGUI_CHECKVERSION();
    auto* ctx = ImGui::CreateContext();
    if (!ctx) { return EXIT_FAILURE; }
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    auto& style = ImGui::GetStyle();
    if (!ImGui_ImplGlfw_InitForOpenGL(win, true)) { return EXIT_FAILURE; }
    if (!ImGui_ImplOpenGL3_Init("#version 460")) { return EXIT_FAILURE; }
*/
    ui::init_imgui_opengl3_glfw(win);
    GLuint program = opengl::create_program(
        fs::path("./vec3pos_vec2uv_MVP.vert"),
        fs::path("./vec2uv.frag")
    );
    GLuint fbuff_program = opengl::create_program(
        fs::path("./vec3pos_MVP.vert"),
        fs::path("./fixed_color.frag")
    );
    std::cout << opengl::get_program_interface(program) << std::endl;
    std::cout << opengl::get_program_interface(fbuff_program) << std::endl;

    const auto& fbuff_input = fbuffer_rect();
    GLuint fbuff_vao = opengl::gen_vertex_array();
    std::vector<GLuint> fbuff_vbo = fbuffer_input_t::gen_buffers(fbuff_vao,
                                                                 fbuff_input);

    opengl::FramebufferData fbuff_data {
        .fbo = opengl::gen_framebuffer(),
        .attachment_point = GL_COLOR_ATTACHMENT0,
        .texture = {
            .id = opengl::gen_texture(),
            .target = GL_TEXTURE_2D,
            .w = GLint(WIDTH),
            .h = GLint(HEIGHT),
            .format = GL_RGBA,
            .type = GL_UNSIGNED_BYTE,
            .wrap_s = GL_CLAMP_TO_EDGE,
            .wrap_t = GL_CLAMP_TO_EDGE,
            .min_filter = GL_LINEAR,
            .mag_filter = GL_LINEAR
        }
    };
    opengl::set_texture_meta(nullptr, fbuff_data.texture);
    opengl::attach_texture(fbuff_data, fbuff_data.texture);

    std::cout << fbuff_data << std::endl;

    const glm::ivec4 screen_view = {0, 0, WIDTH, HEIGHT};

    ui::ImGuiWidgetRender ui_render;
    glm::mat4 one(1.0);
    glm::mat4 fbuff_scale = glm::scale(one, {0.1, 0.1, 0.0});
    while (!glfwWindowShouldClose(win)) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        glfwPollEvents();

        opengl::Context::instance().draw_background();
        opengl::use(fbuff_program);
        opengl::set_mat4(fbuff_program, "projection", one);
        opengl::set_mat4(fbuff_program, "view", one);
        opengl::set_mat4(fbuff_program, "model", fbuff_scale);
        opengl::draw_array_framebuffer({
            .fbo = fbuff_data.fbo,
            .vao = fbuff_vao,
            .count = fbuff_input.size(),
            .viewport = {0, 0, fbuff_data.texture.w, fbuff_data.texture.h},
            .screen_viewport = {0, 0, WIDTH, HEIGHT},
            .background = {1.0, 1.0, 0.0, 0.4}
        });
        opengl::use(0);
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