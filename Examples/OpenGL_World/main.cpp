#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <UI/ui.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/camera.hpp>
#include <Loader/opengl_converter.hpp>

#include "item.hpp"


constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;


glm::vec4 background   = {0.5, 0.8, 0.8, 1.0};
glm::vec4 item_color   = {1.0, 0.5, 0.31, 1.0};
glm::vec4 ground_color = {0.5, 0.5, 0.5, 1.0};
glm::vec4 light_color  = {0.8, 0.8, 1.0, 1.0};
glm::vec3 light_pos    = {20.0, 20.0, 20.0};
std::filesystem::path vertex_path("./vertex_shader.vert");
std::filesystem::path fragment_path("./fragment_shader.frag");


int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }

    auto* window = ui::create_window(WIDTH, HEIGHT, "world");
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background(background);
    opengl::Camera camera(WIDTH, HEIGHT, glm::radians(45.0), {10, 10, 10});
    auto program = opengl::create_program(vertex_path, fragment_path);
    Item3D cube;
    cube.open("./cube.obj");

    auto ground_program = opengl::create_program(vertex_path, fragment_path);
    Item3D ground;
    ground.open("./cube.obj");
    //ground.modify(std::move(move_matrix({1.0, 1.0, 1.0})));

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        opengl::Context::instance().draw_background();

        opengl::use(program);
        opengl::set_vec4(program, "color", item_color);
        opengl::set_vec3(program, "light_position", light_pos);
        opengl::set_vec4(program, "light_color", light_color);
        opengl::set_mat4(program, "view", camera.view());
        opengl::set_mat4(program, "projection", camera.projection());
        //cube.draw(program);
        pass_shader_uniforms(program, {
            .color = item_color,
            .light_position = light_pos,
            .light_color = light_color,
            .view = camera.view(),
            .projection = camera.projection(),
            .model = 
        }, cube);

        //opengl::use(ground_program);
        //opengl::set_vec4(ground_program, "color", ground_color);
        //opengl::set_vec3(ground_program, "light_position", light_pos);
        //opengl::set_vec4(ground_program, "light_color", light_color);
        //opengl::set_mat4(ground_program, "view", camera.view());
        //opengl::set_mat4(program, "projection", camera.projection());
        //ground.draw(ground_program);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
