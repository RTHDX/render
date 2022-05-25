#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ui.hpp"
#include "opengl.hpp"


void process_input(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 860;

const std::string_view vertex_shader_source =
    "#version 460 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const std::string_view fragment_shader_source =
    "#version 460 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";


int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }
    GLFWwindow* window = ui::create_window(SCR_WIDTH, SCR_HEIGHT, "Test");
    glfwSetFramebufferSizeCallback(window, opengl::framebuffer_size_callback);

    opengl::Context::instance().initialize();
    opengl::Context::instance().dump();
    opengl::Context::instance().background({0.2f, 0.5f, 0.5f, 1.0f});

    std::vector<float> vertices {
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
    std::vector<uint32_t> indices {
        0, 1, 3,
        1, 2, 3
    };

    opengl::Pipeline pipeline;
    pipeline.create_vertex_shader(vertex_shader_source);
    pipeline.create_fragment_shader(fragment_shader_source);
    pipeline.create_program();
    pipeline.create_vao();
    pipeline.create_vbo(vertices.data(), vertices.size() * sizeof (float));
    pipeline.create_ebo(indices.data(), indices.size() * sizeof(uint32_t));
    pipeline.set_layot(0, 3, 3);

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        pipeline.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
