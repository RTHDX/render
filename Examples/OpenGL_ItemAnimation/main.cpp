#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <UI/ui.hpp>
#include <OpenGL/camera.hpp>
#include <OpenGL/opengl_utils.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/opengl_vertex_input.hpp>
#include <OpenGL/texture.hpp>
#include <OpenGL/opengl_functions.hpp>
#include <Render/Animation.hpp>

#include "Item.hpp"

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

glm::mat4 create_item_model() {
    return {
        0.1, 0.0, 0.0, 0.0,
        0.0, 0.2, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
}

opengl::TextureArray sprite_atlas() {
    opengl::TextureArray texture(R"(.\hr-level1_running.png)", 22, 8);
    texture.read();
    return texture;
}

std::map<Direction, render::Animation> create_animations() {
    return {
        {Direction::NORTH, {0, 22}},
        {Direction::EAST_NORTH, {23, 45}},
        {Direction::EAST, {45, 67}},
        {Direction::EAST_SOUTH, {67, 89}},
        {Direction::SOUTH, {89, 111}},
        {Direction::SOUTH_WEST, {111, 133}},
        {Direction::WEST, {123, 155}},
        {Direction::WEST_NORTH, {155, 177}}
    };
}

Item_2D create_item() {
    Item_2D out{
        .input = create_vertices(),
        .indices = create_indices(),
        .model = create_item_model(),
        .atlas = sprite_atlas(),
        .animations = create_animations()
    };
    return out;
}

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
        0.1, 0.0, 0.0, 0.0,
        0.0, 0.2, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    auto animations = create_animations();
    uint64_t count = 0;
    while (!glfwWindowShouldClose(window)) {
        auto& animation = animations[Direction::NORTH];
        if (count == 5) {
            animation.update();
            count = 0;
        }
        frame_preprocess(vao);
        opengl::Context::instance().draw_background();
        texture.bind();
        opengl::use(program);
        opengl::set_mat4(program, "projection", projection);
        opengl::set_mat4(program, "view", view);
        opengl::set_mat4(program, "model", model);
        opengl::set_int(program, "total_frames", animation.total_frames());
        render::index_t current_frame = animation.current_index();
        opengl::set_int(program, "current_frame",
                        current_frame);
        opengl::draw(opengl::DrawElementsCommand{
            .vao = vao,
            .count = indices.size()
        });
        frame_postprocess(window);

        count++;
    }

    opengl::free_vertex_buffers(vbo);
    opengl::free_element_buffer(ebo);
    opengl::free_vertex_array(vao);
    glfwTerminate();
    return 0;
}
