#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "UI/ui.hpp"
#include "OpenGL/opengl.hpp"
#include "OpenGL/item.hpp"


void process_input(GLFWwindow* window);
void render(opengl::Program& program);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
    "   FragColor = vec4(1.0f, 0.0f, 0.2f, 1.0f);\n"
    "}\n\0";


int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }
    GLFWwindow* window = ui::create_window(SCR_WIDTH, SCR_HEIGHT, "Test");
    glfwSetFramebufferSizeCallback(window, opengl::framebuffer_size_callback);

    opengl::Context::instance().initialize();
    opengl::Context::instance().dump();
    opengl::Context::instance().background({0.2f, 0.5f, 0.5f, 1.0f});

    const std::vector<float> vertices{
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
    };
    const std::vector<uint32_t> indices{0, 1, 2};

    opengl::Program program("orange");
    program.attach_shader(GL_VERTEX_SHADER, vertex_shader_source);
    program.attach_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
    program.link_program();

    opengl::Item triangle(vertices, indices, opengl::AttribPointer(0, 3, 3));

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        opengl::Context::instance().draw_background();
        triangle.draw(program);

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
