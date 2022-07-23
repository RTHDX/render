#include <filesystem>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <UI/ui.hpp>

#include <OpenGL/opengl_utils.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/opengl_vertex_input.hpp>

using VertexData = opengl::vec3pos_vec2tex_t;
using VertexAttrib = opengl::VertexAttribCommand<VertexData>;

int WIDTH = 1280;
int HEIGHT = 860;


std::vector<glm::vec3> gen_frame(size_t width, size_t height,
                                 const glm::vec3& color={}) {
    std::vector<glm::vec3> out(width * height);
    for (size_t h_pos = 0; h_pos < height; ++h_pos) {
        for (size_t w_pos = 0; w_pos < width; ++w_pos) {
            const size_t i = h_pos * width + w_pos;
            if (i % 2 == 0) {
                out[i] = color == glm::vec3{} ? glm::vec3{.5, .9, .5} : color;
            } else {
                out[i] = color == glm::vec3{} ? glm::vec3{.9, .5, .5} : color;
            }
        }
    }
    return out;
}

glm::vec3 make_color(float radians) {
    return {
        sin(radians),
        cos(radians),
        sin(radians)
    };
}


int main() {
    if (!ui::init_glfw(4, 6)) {
        std::cerr << "Could not load glfw" << std::endl;
        return EXIT_FAILURE;
    }

    auto* window = ui::create_window(WIDTH, HEIGHT, "Textures");
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background({0.8, 0.1, 0.1, 0.1});

    auto program = opengl::create_program(
        std::filesystem::path("./vertex_shader.vert"),
        std::filesystem::path("./fragment_shader.frag")
    );

    std::vector<VertexData> vertices {
        {.pos{-1.0,  1.0, 0.0}, .tex_pos={0, 1}},
        {.pos{ 1.0,  1.0, 0.0}, .tex_pos={1, 1}},
        {.pos{ 1.0, -1.0, 0.0}, .tex_pos={1, 0}},

        {.pos{-1.0,  1.0, 0.0}, .tex_pos={0, 1}},
        {.pos{-1.0, -1.0, 0.0}, .tex_pos={0, 0}},
        {.pos{1.0,  -1.0, 0.0}, .tex_pos={1, 0}}
    };
    auto vao = opengl::gen_vertex_array();
    auto pos_vbo = opengl::gen_vertex_buffers();
    auto tex_vbo = opengl::gen_vertex_buffers();
    opengl::bind_vao(vao);
    opengl::bind_vbo<VertexData>(pos_vbo, vertices);
    opengl::bind_vbo<VertexData>(tex_vbo, vertices);
    opengl::do_vertex_attrib_cmds<VertexData>(
    {
        {.index=0, .stride=3, .offset=(void*)offsetof(VertexData, pos)},
        {.index=1, .stride=2, .offset=(void*)offsetof(VertexData, tex_pos)}
    });

    auto tex = opengl::gen_texture();

    float radians = 0.0;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        opengl::Context::instance().draw_background();

        opengl::bind_texture(
            {WIDTH, HEIGHT},
            std::move(gen_frame(WIDTH, HEIGHT, make_color(radians)))
        );
        radians += 0.01;
        opengl::activate_texture(tex);
        opengl::use(program);
        opengl::draw({.vao = vao, .count = vertices.size()});

        glfwSwapBuffers(window);
    }

    opengl::free_vertex_array(vao);
    opengl::free_vertex_buffer(pos_vbo);
    opengl::free_vertex_buffer(tex_vbo);
    opengl::free_program(program);

    glfwTerminate();
    return EXIT_SUCCESS;
}
