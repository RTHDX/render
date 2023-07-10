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
#include <OpenGL/texture.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/opengl_vertex_input.hpp>
#include <OpenGL/camera.hpp>

#include <UI/io.hpp>

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

struct Window {
    ImVec2 top_left;
    ImVec2 win_sizes;
    std::string title;
    int flags = 0;
};

ImVec2 convert_to_abs(const ImVec2& parent, const ImVec2& rel) {
    return {
        parent.x * (rel.x / 100.0f),
        parent.y * (rel.y / 100.0f)
    };
}

void draw_imgui_window(const glm::vec4& screen_viewport,
                       const opengl::FramebufferData& fbuff_data,
                       const Window& win) {
    ImVec2 total_size(screen_viewport.z, screen_viewport.w);
    ImGui::SetNextWindowPos(
        convert_to_abs(total_size, win.top_left),
        ImGuiCond_Always
    );

    auto win_size = convert_to_abs(total_size, win.win_sizes);
    ImGui::SetNextWindowSize(win_size, ImGuiCond_Always);

    ImGui::Begin(win.title.data(), nullptr, win.flags);

    ImTextureID texID = reinterpret_cast<void*>(
        static_cast<intptr_t>(fbuff_data.texture.id)
    );
    //ImVec2 texSize(static_cast<float>(fbuff_data.texture.w),
    //               static_cast<float>(fbuff_data.texture.h));
    ImVec2 tex_sizes(win_size.x * 0.98, win_size.y * 0.93);
    ImVec2 tex_min(0, 1);
    ImVec2 tex_max(1, 0);
    ImGui::Image(texID, tex_sizes, tex_min, tex_max);

    ImGui::End();
}

int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }

    auto* win = ui::create_window(WIDTH, HEIGHT,
        "Framebuffer rendering to imgui window"
    );
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background(glm::vec4{0.8, 1.0, 0.0, 1.0 });
    ui::io::IO::instance().bind(win);

    IMGUI_CHECKVERSION();
    auto* ctx = ImGui::CreateContext();
    if (!ctx) { return EXIT_FAILURE; }
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    auto& style = ImGui::GetStyle();
    if (!ImGui_ImplGlfw_InitForOpenGL(win, true)) { return EXIT_FAILURE; }
    if (!ImGui_ImplOpenGL3_Init("#version 460")) { return EXIT_FAILURE; }

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

    Window im_win {
        .top_left  = {2, 2},
        .win_sizes = {96, 96},
        .title     = "Framebuffer window",
        .flags     = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
    };
    const glm::ivec4 screen_view = {0, 0, WIDTH, HEIGHT};

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
        draw_imgui_window(screen_view, fbuff_data, im_win);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(win);
    }

    glfwDestroyWindow(win);
    glfwTerminate();
    return EXIT_SUCCESS;
}