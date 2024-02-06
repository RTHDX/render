#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <UI/ui.hpp>
#include <OpenGL/texture.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/opengl_vertex_input.hpp>

namespace fs = std::filesystem;
using vertex_t = opengl::vec3pos_vec2tex_t;

std::vector<vertex_t> create_vertices_xz() {
    return {
        {{-1.0, 0.0, -1.0}, {0, 1}},
        {{ 1.0, 0.0, -1.0}, {1, 1}},
        {{ 1.0, 0.0,  1.0}, {1, 0}},
        {{-1.0, 0.0,  1.0}, {0, 0}}
    };
}

std::vector<GLuint> create_indices() {
    return {
        3, 0, 1,
        1, 2, 3
    };
}

const GLuint SCR_WIDTH  = 800;
const GLuint SCR_HEIGHT = 800;
const GLint T_W = 800, T_H = 800;


void update_ortho(glm::vec4& clip_space, float d) {
    clip_space.x -= d;
    clip_space.y += d;
    clip_space.z -= d;
    clip_space.w += d;
}

glm::mat4 get_ortho_projection(const glm::vec4& space) {
    return glm::ortho(
        space.x, space.y,
        space.z, space.w,
        0.1f, 100.0f
    );
}

glm::mat4 get_perspective_projection() {
    return glm::perspective(
        glm::radians(90.0f),
        float(SCR_WIDTH) / float(SCR_HEIGHT),
        0.1f, 100.0f
    );
}


int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }

    GLFWwindow* window = ui::create_window(SCR_WIDTH, SCR_HEIGHT,
        "OpenGL_OrthoCamTexture"
    );
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background(glm::vec4{0.8, 0.8, 0.8, 1.0});
    auto program = opengl::create_program(
        fs::path("pos3_uv2.vert"), fs::path("samp2d.frag")
    );
    std::cout << opengl::get_program_interface(program) << std::endl;

    auto vao = opengl::gen_vertex_array();
    auto ebo = opengl::gen_element_buffer();
    auto vbos = vertex_t::gen_buffers(vao, create_vertices_xz(),
                                      ebo, create_indices());
    opengl::TextureData tex_data {
        .id         = opengl::gen_texture(),
        .target     = GL_TEXTURE_2D,
        .w          = 1,
        .h          = 1,
        .format     = GL_RGBA,
        .type       = GL_UNSIGNED_BYTE,
        .wrap_s     = GL_CLAMP_TO_EDGE,
        .wrap_t     = GL_CLAMP_TO_EDGE,
        .min_filter = GL_LINEAR,
        .mag_filter = GL_LINEAR
    };

    glm::u8vec4 color (255, 0, 0, 255);
    std::vector<glm::u8vec4> image(1, color);
    opengl::set_texture_meta(glm::value_ptr(*(image.data())), tex_data);

    glm::vec4 ortho_corners{
        -2.0f, 2.0f,
        -2.0f, 2.0f,
    };
    glm::mat4 projection = get_ortho_projection(ortho_corners);

    glm::vec3 pos = {0.0, 5.0, 0.0};
    glm::vec3 center = {0.0, 0.0, 0.0};
    glm::vec3 up = {0.0, 0.0, 1.0};
    glm::mat4 view = glm::lookAt(pos, center, up);

    glm::mat4 model(1.0);
    while (!glfwWindowShouldClose(window)) {
        SAFE_CALL(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));

        auto action = glfwGetKey(window, GLFW_KEY_ESCAPE);
        if (action == GLFW_PRESS) { glfwSetWindowShouldClose(window, true); }
        opengl::Context::instance().draw_background();

        opengl::use(program);
        opengl::activate_texture({
            .tex_unit     = GL_TEXTURE0,
            .sampler_type = tex_data.target,
            .id           = tex_data.id,
            .program      = program,
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
