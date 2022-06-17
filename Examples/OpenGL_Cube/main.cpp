#include <iostream>
#include <filesystem>

#include <glm/gtc/matrix_transform.hpp>

#include <ui.hpp>
#include <opengl.hpp>
#include <item.hpp>
#include <camera.hpp>
#include <opengl_utils.hpp>
#include <application.hpp>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;


std::vector<float> vertices{
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
};


int main() {
    ui::Application& app = ui::Application::instance();
    auto* window = app.window();

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

    opengl::Item cube(vertices, opengl::AttribPointer(0, 3, 3));
    opengl::Camera camera(WIDTH, HEIGHT, glm::radians(45.0));
    //app.subscribe(&camera);
    //camera.bind(&app);

    while (!glfwWindowShouldClose(window)) {
        opengl::Context::instance().draw_background();

        cube.draw(program);

        program.set_mat4("model", cube.model());
        //program.set_mat4("view", camera.view());
        //program.set_mat4("projection", camera.projection());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
