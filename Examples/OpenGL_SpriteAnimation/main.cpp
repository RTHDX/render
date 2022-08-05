#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <UI/ui.hpp>
#include <OpenGL/camera.hpp>
#include <OpenGL/opengl_utils.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/opengl_vertex_input.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "3rdParty/stb/stb_image.h"

int WIDTH = 1280;
int HEIGHT = 860;
const std::filesystem::path vertex_path(R"(.\vertex_shader.vert)");
const std::filesystem::path fragment_path(R"(.\fragment_shader.frag)");
const std::filesystem::path image_path(R"(.\hr-level1_running.png)");
glm::vec4 background = { 0.5, 0.8, 0.8, 1.0 };

using byte_t = unsigned char;
using VertexData = opengl::vec3pos_vec3norm_vec2tex_t;

std::vector<VertexData> create_square() {
    glm::vec3 norm(0.0, 0.0, 1.0);
    return {
        {.pos{-1.0, 1.0,  0.0}, .norm=norm, .tex_pos{0, 1}},
        {.pos{ 1.0, 1.0,  0.0}, .norm=norm, .tex_pos{1, 1}},
        {.pos{ 1.0, -1.0, 0.0}, .norm=norm, .tex_pos{1, 0}},

        {.pos{-1.0,  1.0, 0.0}, .norm=norm, .tex_pos{0, 1}},
        {.pos{-1.0, -1.0, 0.0}, .norm=norm, .tex_pos{0, 0}},
        {.pos{1.0,  -1.0, 0.0}, .norm=norm, .tex_pos{1, 0}}
    };
}

int main() {
    if (!ui::init_glfw(4, 6)) {
        return EXIT_FAILURE;
    }

    auto* window = ui::create_window(WIDTH, HEIGHT, "Sprite animation");
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background(background);

    opengl::Camera camera(WIDTH, HEIGHT, glm::radians(45.0), {0, 0, 5});

    auto program = opengl::create_program(vertex_path, fragment_path);

    GLuint vao = opengl::gen_vertex_array(),
           pos_vbo = opengl::gen_vertex_buffers(),
           norm_vbo = opengl::gen_vertex_buffers(),
           tex_vbo = opengl::gen_vertex_buffers();

    auto vertices = create_square();

    opengl::bind_vao(vao);
    opengl::bind_vbo<VertexData>(pos_vbo, vertices);
    opengl::bind_vbo<VertexData>(norm_vbo, vertices);
    opengl::bind_vbo<VertexData>(tex_vbo, vertices);
    opengl::do_vertex_attrib_cmds<VertexData>({
        {.index=0, .stride=3, .offset=(void*)offsetof(VertexData, pos)},
        {.index=1, .stride=3, .offset=(void*)offsetof(VertexData, norm)},
        {.index=2, .stride=2, .offset=(void*)offsetof(VertexData, tex_pos)}
    });

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        opengl::use(program);
        opengl::draw({.vao=vao, .count=vertices.size()});

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
