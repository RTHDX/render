#include <iostream>
#include <filesystem>

#include <ui.hpp>
#include <opengl.hpp>
#include <opengl_utils.hpp>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;


int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }
    auto* window = ui::create_window(WIDTH, HEIGHT, "Cube");

    opengl::Context::instance().initialize();
    opengl::Context::instance().dump();
    opengl::Context::instance().background({0.2f, 0.5f, 0.5f, 1.0f});

    const std::filesystem::path vertex_path(R"(.\vertex_shader.vert)");
    const std::filesystem::path fragment_path(R"(.\fragment_shader.frag)");
    opengl::Program program("cube");
    program.attach_shader(GL_VERTEX_SHADER,
                          opengl::utils::read_shader(vertex_path));
    program.attach_shader(GL_FRAGMENT_SHADER,
                          opengl::utils::read_shader(fragment_path));
    program.link_program();

    return EXIT_SUCCESS;
}
