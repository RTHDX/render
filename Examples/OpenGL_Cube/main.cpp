#include <iostream>
#include <filesystem>

#include <glm/gtc/matrix_transform.hpp>

#include <UI/ui.hpp>
#include <OpenGL/opengl.hpp>
#include <OpenGL/item.hpp>
#include <OpenGL/camera.hpp>
#include <OpenGL/opengl_utils.hpp>
#include <UI/application.hpp>
#include <UI-Nuklear/opengl_application.hpp>

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


auto create_camera() {
    return opengl::Camera {
        WIDTH, HEIGHT, glm::radians(45.0)
    };
}

int main() {
    ui::nuklear::OpenGL_App app("cube", WIDTH, HEIGHT,
                                std::move(create_camera()));
    app.initialize();

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
    auto callback = [&app, &program, &cube]() {
        cube.draw(program);
        program.set_mat4("model", cube.model());
        program.set_mat4("view", app.camera().view());
        program.set_mat4("projection", app.camera().projection());
    };

    app.run(callback);
    return EXIT_SUCCESS;
}
