#include <filesystem>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <UI/ui.hpp>

#include <OpenGL/opengl.hpp>
#include <OpenGL/opengl_utils.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/opengl_vertex_input.hpp>

using VertexData = opengl::vec3pos_vec2tex_t;
using VertexAttrib = opengl::VertexAttribCommand<VertexData>;

int WIDTH = 1280;
int HEIGHT = 860;


std::vector<glm::vec3> gen_frame(size_t width, size_t height) {
    std::vector<glm::vec3> out(width * height);
    for (size_t h_pos = 0; h_pos < height; ++h_pos) {
        for (size_t w_pos = 0; w_pos < width; ++w_pos) {
            const size_t i = h_pos * width + w_pos;
            out[i] = {.5, .5, .5};
        }
    }
    return out;
}

struct TexVertexData {
    glm::vec3 pos;
    glm::vec2 tex_pos;
};


GLuint create_texture() {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    auto frame = gen_frame(WIDTH, HEIGHT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT,
                 glm::value_ptr(*frame.data()));
    return tex;
}


int main() {
    if (!ui::init_glfw(4, 6)) {
        std::cerr << "Could not load glfw" << std::endl;
        return EXIT_FAILURE;
    }

    auto* window = ui::create_window(WIDTH, HEIGHT, "Textures");
    opengl::Context::instance().initialize(true);

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
    opengl::do_vertex_attrib_cmds<VertexData>(vao, {
        {.index=0, .stride=3, .offset=(void*)offsetof(VertexData, pos)},
        {.index=1, .stride=2, .offset=(void*)offsetof(VertexData, tex_pos)}
    });

    GLuint tex = create_texture();

    opengl::DrawArrayCommand draw_cmd{
        .vao = vao,
        .count = vertices.size()
    };
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        opengl::Context::instance().draw_background();

        glBindTexture(GL_TEXTURE_2D, tex);
        glUseProgram(program);

        opengl::draw(draw_cmd);

        glfwSwapBuffers(window);
    }

    opengl::free_vertex_array(vao);
    opengl::free_vertex_buffer(pos_vbo);
    opengl::free_vertex_buffer(tex_vbo);

    glfwTerminate();
    return EXIT_SUCCESS;
}
