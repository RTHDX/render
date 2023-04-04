#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>

#include <UI/ui.hpp>
#include <OpenGL/texture.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/opengl_vertex_input.hpp>

namespace fs = std::filesystem;
using vertex_t = opengl::vec3pos_vec2tex_t;

std::vector<vertex_t> create_vertices() {
    return {
        {{ 1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}}, // top right
        {{ 1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}}, // bottom right
        {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}}, // bottom left
        {{-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}}  // top left 
    };
}

std::vector<GLuint> create_indices() {
    return {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
}

const GLuint SCR_WIDTH  = 800;
const GLuint SCR_HEIGHT = 800;
const GLint T_W = 200, T_H = 200;


opengl::byte_t* create_texture(const glm::vec4& color, int w, int h) {
    const size_t len = w * h;
    opengl::byte_t* data = new opengl::byte_t [len * 4];
    size_t i = 0;
    while (i < len) {
        data[i++] = 255 * color.r;
        data[i++] = 255 * color.g;
        data[i++] = 255 * color.b;
        data[i++] = 255 * color.a;
    }
    return data;
}


int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }

    GLFWwindow* window = ui::create_window(SCR_WIDTH, SCR_HEIGHT,
        "OpenGL_OrthoCamTexture"
    );
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background({0.8, 0.8, 0.8, 1.0});
    auto program = opengl::create_program(
        fs::path("pos3_uv2.vert"), fs::path("samp2d.frag")
    );
    std::cout << opengl::get_program_interface(program) << std::endl;

    auto vao = opengl::gen_vertex_array();
    auto ebo = opengl::gen_element_buffer();
    auto vbos = vertex_t::gen_buffers(vao, create_vertices(),
                                      ebo, create_indices());
    opengl::TextureData tex_data {
        .id         = opengl::gen_texture(),
        .target     = GL_TEXTURE_2D,
        .w          = T_W,
        .h          = T_H,
        .format     = GL_RGBA,
        .type       = GL_UNSIGNED_BYTE,
        .wrap_s     = GL_CLAMP_TO_EDGE,
        .wrap_t     = GL_CLAMP_TO_EDGE,
        .min_filter = GL_LINEAR,
        .mag_filter = GL_LINEAR
    };
    opengl::byte_t* raw_tex_data = create_texture({0.0, 1.0, 0.0, 1.0}, T_W,
                                                  T_H);
    opengl::set_texture_meta(raw_tex_data, tex_data);
    delete [] raw_tex_data;

    glm::mat4 projection = glm::ortho(
        -2.0f, 2.0f,
        -2.0f, 2.0f,
        0.001f, 100.0f
    );
    float delta = 0.01;
    glm::mat4 view = glm::lookAt(
        glm::vec3(delta, 2.0f - delta, delta),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    glm::mat4 model (1.0);
    while (!glfwWindowShouldClose(window)) {
        auto action = glfwGetKey(window, GLFW_KEY_ESCAPE);
        if (action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        opengl::Context::instance().draw_background();

        opengl::use(program);
        opengl::activate_texture({
            .tex_type = GL_TEXTURE0,
            .sampler_type = tex_data.target,
            .id = tex_data.id,
            .program = program,
            .sampler_name = "texture_sampler"
        });
        opengl::set_mat4(program, "projection", projection);
        opengl::set_mat4(program, "view", view);
        opengl::set_mat4(program, "model", model);
        opengl::draw(opengl::DrawElementsCommand {
            .vao = vao,
            .count = 6
        });

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
