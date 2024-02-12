#include <array>
#include <vector>
#include <filesystem>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
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


using scene_input_t   = opengl::vec3pos_vec2tex_t;
using fbuffer_input_t = opengl::vec3pos;

const std::vector<scene_input_t>& main_rect() {
    static std::vector<scene_input_t> data {
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

float rot_value() {
    static float rotation = 0.0;
    static constexpr float rot_speed = 0.01;
    rotation += rot_speed;
    return (std::sin(rotation));
}

float rot_vertically_value() {
    static constexpr float rot_speed = 0.02;
    static float rotation = 0.0;
    rotation += rot_speed;
    return std::cos(rotation);
}


int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }

    auto* win = ui::create_window(WIDTH, HEIGHT, "Framebuffer rendering");
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background(glm::vec4{ 0.0, 0.8, 0.0, 1.0 });
    ui::io::IO::instance().bind(win);

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

    const auto& scene_input = main_rect();
    GLuint vao = opengl::gen_vertex_array();
    std::vector<GLuint> vbo = scene_input_t::gen_buffers(vao, scene_input);

    const auto& fbuff_input = fbuffer_rect();
    GLuint fbuff_vao = opengl::gen_vertex_array();
    std::vector<GLuint> fbuff_vbo = fbuffer_input_t::gen_buffers(fbuff_vao,
                                                                 fbuff_input);
    opengl::framebuffer_data_t fbuff_data {
        .fbo              = opengl::gen_framebuffer(),
        .attachment_point = GL_COLOR_ATTACHMENT0,
        .texture          = {
            .id         = opengl::gen_texture(),
            .target     = GL_TEXTURE_2D,
            .w          = GLint(WIDTH * 0.5f),
            .h          = GLint(HEIGHT),
            .format     = GL_RGBA,
            .type       = GL_UNSIGNED_BYTE,
            .wrap_s     = GL_CLAMP_TO_EDGE,
            .wrap_t     = GL_CLAMP_TO_EDGE,
            .min_filter = GL_LINEAR,
            .mag_filter = GL_LINEAR
        }
    };
    opengl::set_texture_meta(nullptr, fbuff_data.texture);
    opengl::attach_texture(fbuff_data, fbuff_data.texture);

    glm::mat4 one(1.0);
    const glm::mat4 fbuff_scale = glm::scale(one, {0.5, 0.5, 0.5});
    const glm::mat4 main_scale = glm::scale(one, {0.5, 1.0, 0.0});
    glm::mat4 main_model = one;
    glm::mat4 fbuff_model = one;
    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        opengl::Context::instance().draw_background();

        fbuff_model = glm::translate(one, {-rot_value(), rot_vertically_value(), 0.0});
        opengl::use(fbuff_program);
        opengl::set_mat4(fbuff_program, "projection", one);
        opengl::set_mat4(fbuff_program, "view", one);
        opengl::set_mat4(fbuff_program, "model", fbuff_model * fbuff_scale);
        opengl::draw_array_framebuffer({
            .fbo = fbuff_data.fbo,
            .vao = fbuff_vao,
            .count = fbuff_input.size(),
            .viewport = {0, 0, fbuff_data.texture.w, fbuff_data.texture.h},
            .screen_viewport = {0, 0, WIDTH, HEIGHT},
            .background = {0.0, 0.0, 0.0, 0.3}
        });
        opengl::use(0);

        main_model = glm::translate(one, {rot_value(), 0.0, 0.0});
        opengl::use(program);
        opengl::set_mat4(program, "projection", one);
        opengl::set_mat4(program, "view", one);
        opengl::set_mat4(program, "model", main_model * main_scale);
        opengl::activate_texture({
            .tex_unit     = GL_TEXTURE0,
            .sampler_type = fbuff_data.texture.target,
            .id           = fbuff_data.texture.id,
            .program      = program,
            .sampler_name = "tex"
        });
        opengl::draw(opengl::DrawArrayCommand{
            .vao = vao,
            .count = scene_input.size()
        });
        opengl::use(0);
        glfwSwapBuffers(win);
    }

    glfwDestroyWindow(win);
    glfwTerminate();
}