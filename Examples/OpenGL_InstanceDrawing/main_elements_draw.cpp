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

using vertex_input_t = opengl::vec3pos_vec2tex_t;
using instanced_input_t = opengl::mat4_instanced;

static constexpr int WIDTH = 800;
static constexpr int HEIGHT = 600;
static constexpr size_t SQUARES = 4;
static constexpr float MOVE = 0.51;
static constexpr float CELL_SIZE = 1.0;
static constexpr glm::mat4 ONE(1.0);

static glm::mat4 SCALE_MAT = glm::scale(ONE, glm::vec3(0.4));

const std::vector<vertex_input_t>& vertex_input() {
    static std::vector<vertex_input_t> out {
        {{ CELL_SIZE,  CELL_SIZE, 0.0f}, {1.0f, 1.0f}},
        {{ CELL_SIZE, -CELL_SIZE, 0.0f}, {1.0f, 0.0f}},
        {{-CELL_SIZE, -CELL_SIZE, 0.0f}, {0.0f, 0.0f}},
        {{-CELL_SIZE,  CELL_SIZE, 0.0f}, {0.0f, 1.0f}}
    };
    return out;
}

const std::vector<GLuint> vertex_indices() {
    static std::vector<GLuint> out {
        0, 1, 3,
        1, 2, 3
    };
    return out;
}

const std::vector<instanced_input_t>& instanced_input() {
    static std::vector<instanced_input_t> out {
        {glm::translate(ONE, glm::vec3(-MOVE,  MOVE, 0.0f)) * SCALE_MAT},
        {glm::translate(ONE, glm::vec3( MOVE,  MOVE, 0.0f)) * SCALE_MAT},
        {glm::translate(ONE, glm::vec3(-MOVE, -MOVE, 0.0f)) * SCALE_MAT},
        {glm::translate(ONE, glm::vec3( MOVE, -MOVE, 0.0f)) * SCALE_MAT}
    };
    return out;
}

int main() {
    if (!ui::init_glfw(4, 6)) { return 1; }

    auto* win = ui::create_window(WIDTH, HEIGHT, "Instance Element Drawing");
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background(glm::vec4{0.2, 0.6, 1.0, 1.0});
    ui::io::IO::instance().bind(win);

    auto program = opengl::create_program(
        fs::path("./vec3pos_vec2tex_mat4mod.vert"),
        fs::path("./vec2tex_samp2D.frag")
    );
    std::cout << opengl::get_program_interface(program) << std::endl;

    const auto& vertices = vertex_input();
    const auto& elements = vertex_indices();
    const auto& instances = instanced_input();

    GLuint vao = opengl::gen_vertex_array(),
           ebo = opengl::gen_element_buffer();
    auto buffers = vertex_input_t::gen_buffers(vao, vertices, ebo, elements);
    auto i_buffer = instanced_input_t::gen_buffer(vao, instances, 2);

    glm::mat4 proj(1.0), view(1.0);
    const opengl::TextureData tex_data{
        .id = opengl::gen_texture(GL_TEXTURE_2D),
        .target = GL_TEXTURE_2D,
        .w = 1,
        .h = 1,
        .format = GL_RGBA,
        .type = GL_UNSIGNED_BYTE,
        .wrap_s = GL_CLAMP_TO_EDGE,
        .wrap_t = GL_CLAMP_TO_EDGE,
        .min_filter = GL_LINEAR,
        .mag_filter = GL_LINEAR
    };

    const opengl::TextureActivationCommand tex_activation{
        .tex_unit = GL_TEXTURE0,
        .sampler_type = tex_data.target,
        .id = tex_data.id,
        .program = program,
        .sampler_name = "texture_1"
    };

    glm::u8vec4 color{ 255, 0, 0, 255 };
    opengl::set_texture_meta(glm::value_ptr(color), tex_data);

    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        opengl::Context::instance().draw_background();

        opengl::use(program);
        opengl::activate_texture(tex_activation);
        opengl::set_mat4(program, "projection", proj);
        opengl::set_mat4(program, "view", view);
        opengl::draw_instance_elements({
            .vao = vao,
            .count = GLsizei(elements.size()),
            .instancecount = GLsizei(instances.size())
        });
        opengl::use(0);

        glfwSwapBuffers(win);
    }

    opengl::free_vertex_buffers(buffers);
    opengl::free_vertex_buffer(i_buffer);
    opengl::free_element_buffer(ebo);
    opengl::free_vertex_array(vao);
    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}