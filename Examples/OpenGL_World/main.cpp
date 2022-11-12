#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <UI/ui.hpp>
#include <UI/io.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/camera.hpp>
#include <Loader/opengl_converter.hpp>

#include "io.hpp"
#include "item.hpp"


constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;


glm::vec4 background   = {0.5, 0.8, 0.8, 1.0};
glm::vec4 ground_color = {0.5, 0.5, 0.5, 1.0};
glm::vec4 light_color  = {0.8, 0.8, 1.0, 1.0};
glm::vec3 light_pos    = {20.0, 20.0, 20.0};
std::filesystem::path vertex_path("./vertex_shader.vert");
std::filesystem::path fragment_path("./fragment_shader.frag");


int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }

    auto* window = ui::create_window(WIDTH, HEIGHT, "world");
    ui::io::IO::instance().bind(window);
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background(background);
    opengl::Camera camera(WIDTH, HEIGHT, glm::radians(45.0), {10, 10, 10});

    Item3D cube(vertex_path, fragment_path, {1.0, 0.5, 0.31, 1.0});
    cube.open("./cube.obj");

    GlobalListener g_listener(cube, &ui::io::IO::instance());

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        opengl::Context::instance().draw_background();

        pass_shader_uniforms(cube.program(), {
            .color          = cube.color(),
            .light_position = light_pos,
            .light_color    = light_color,
            .view           = camera.view(),
            .projection     = camera.projection(),
            .model          = cube.model()
        }, cube);
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
