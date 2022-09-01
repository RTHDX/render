#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <UI/ui.hpp>
#include <OpenGL/camera.hpp>
#include <OpenGL/opengl_utils.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/opengl_vertex_input.hpp>
#include <OpenGL/texture.hpp>
#include <OpenGL/opengl_functions.hpp>

#include "Animation.hpp"

int WIDTH = 1280;
int HEIGHT = 860;
const std::filesystem::path vertex_path(R"(.\vertex_shader.vert)");
const std::filesystem::path fragment_path(R"(.\fragment_shader.frag)");
glm::vec4 background = {0.5, 0.8, 0.8, 1.0};

void frame_preprocess(GLuint id) {
    opengl::bind_vao(id);
    glfwPollEvents();
}

void frame_postprocess(GLFWwindow* w) {
    glfwSwapBuffers(w);
    opengl::bind_vao(0);
}

std::vector<opengl::vec3pos_vec2tex_t> create_vertices() {
    return {
        {{ 1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}},
        {{ 1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
        {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}}
    };
}

std::vector<uint32_t> create_indices() {
    return {
        0, 1, 3,
        1, 2, 3
    };
}

enum class Direction {
    NORTH,
    EAST_NORTH,
    EAST,
    EAST_SOUTH,
    SOUTH,
    SOUTH_WEST,
    WEST,
    WEST_NORTH
};

struct Item_2D {
    std::map<Direction, render::Animation> animations;
    glm::mat4 model;

public:
    Item_2D() {}
};


int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }
    auto* window = ui::create_window(WIDTH, HEIGHT, "Item Animation");
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background(background);

    auto vertices = create_vertices();
    auto indices = create_indices();

    auto program = opengl::create_program(vertex_path, fragment_path);
    auto vao = opengl::gen_vertex_array();
    auto ebo = opengl::gen_element_buffer();
    auto vbo = decltype(vertices)::value_type::gen_buffers(vao, vertices);
    opengl::bind_vao(vao);
    opengl::bind_ebo(ebo, indices);
    opengl::TextureArray texture(R"(.\hr-level1_running.png)", 22, 8);
    if (!texture.read()) { return EXIT_FAILURE; }

    glm::mat4 projection(1.0);
    glm::mat4 view(1.0);
    glm::mat4 model{
        0.2, 0.0, 0.0, 0.0,
        0.0, 0.4, 0.0, 0.0,
        0.0, 0.0, 0.2, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    render::Animation animation_1({
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
        11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21
    });
    while (!glfwWindowShouldClose(window)) {
        frame_preprocess(vao);
        opengl::Context::instance().draw_background();
        texture.bind();
        opengl::use(program);
        opengl::set_mat4(program, "projection", projection);
        opengl::set_mat4(program, "view", view);
        opengl::set_mat4(program, "model", model);
        opengl::set_int(program, "total_frames", texture.total_tiles());
        opengl::set_int(program, "current_frame", animation_1.current_index());
        opengl::draw(opengl::DrawElementsCommand{
            .vao = vao,
            .count = indices.size()
        });
        animation_1.update();
        frame_postprocess(window);
    }

    opengl::free_vertex_buffers(vbo);
    opengl::free_element_buffer(ebo);
    opengl::free_vertex_array(vao);
    glfwTerminate();
    return 0;
}
