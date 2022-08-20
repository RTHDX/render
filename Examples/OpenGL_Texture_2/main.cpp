#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <UI/ui.hpp>
#include <OpenGL/texture.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/opengl_vertex_input.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

std::vector<opengl::vec3pos_vec3norm_vec2tex_t> create_vertices() {
    return {
        {{ 1.0f,  1.0f, 0.0f},   {1.0f, 0.0f, 0.0f},   {1.0f, 1.0f}}, // top right
        {{ 1.0f, -1.0f, 0.0f},   {0.0f, 1.0f, 0.0f},   {1.0f, 0.0f}}, // bottom right
        {{-1.0f, -1.0f, 0.0f},   {0.0f, 0.0f, 1.0f},   {0.0f, 0.0f}}, // bottom left
        {{-1.0f,  1.0f, 0.0f},   {1.0f, 1.0f, 0.0f},   {0.0f, 1.0f}}  // top left 
    };
}

std::vector<unsigned int> create_indices() {
    return {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
}



int main() {
    if (!ui::init_glfw(4, 6)) {
        return EXIT_FAILURE;
    }
    GLFWwindow* window = ui::create_window(SCR_WIDTH, SCR_HEIGHT, "Texture_2");
    opengl::Context::instance().initialize(true);
    auto program = opengl::create_program(
        std::filesystem::path("vertex_shader.vert"),
        std::filesystem::path("fragment_shader.frag")
    );
    auto vao = opengl::gen_vertex_array();
    auto ebo = opengl::gen_element_buffer();
    auto vertices = create_vertices();
    auto indices = create_indices();
    opengl::bind_vao(vao);
    auto buffers = decltype(vertices)::value_type::gen_buffers(vao, vertices);
    opengl::bind_vao(vao);
    opengl::bind_ebo(ebo, indices);

    opengl::Texture tex("container.jpg");
    if (tex.read()) {
        tex.id = opengl::gen_texture();
        opengl::bind_texture({tex.width, tex.height}, tex.buffer);
    }

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        opengl::Context::instance().draw_background();
        opengl::activate_texture(tex.id);
        opengl::use(program);
        opengl::draw(opengl::DrawElementsCommand{.vao=vao, .count=6});

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    opengl::free_vertex_buffers(buffers);
    opengl::free_element_buffer(ebo);
    opengl::free_vertex_array(vao);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
