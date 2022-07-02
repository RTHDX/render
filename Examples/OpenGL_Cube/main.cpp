#include <iostream>
#include <filesystem>

#include <glm/gtc/matrix_transform.hpp>

#include <UI/ui.hpp>
#include <OpenGL/opengl.hpp>
#include <OpenGL/item.hpp>
#include <OpenGL/camera.hpp>
#include <OpenGL/opengl_utils.hpp>
#include <Loader/opengl_converter.hpp>
#include <UI/application.hpp>
#include <UI/io.hpp>
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

auto read_cube() {
    auto items = loader::Converter().read("./cube.obj");
    for (auto& item : items) { item.initialize(); }
    return items;
}

static void on_key(GLFWwindow*, int k, int s, int a, int m) {
    ui::io::IO::instance().emit(ui::KeyEvent(k, s, a, m));
}

static void on_mouse_moved(GLFWwindow*, double x, double y) {
    ui::io::IO::instance().emit(ui::MouseEvent(x, y));
}

static void on_mouse_pressed(GLFWwindow*, int b, int a, int m) {
    ui::io::IO::instance().emit(ui::MouseButtonEvent(b, a, m));
}

static void on_scrolled(GLFWwindow*, double x, double y) {
    ui::io::IO::instance().emit(ui::ScrollEvent(x, y));
}

static void on_dropped(GLFWwindow*, int count, const char** paths) {
    ui::io::IO::instance().emit(ui::DropEvent(count, paths));
}

auto create_camera() {
    opengl::Camera cam(WIDTH, HEIGHT, glm::radians(45.0), {10, 10, 10});
    return cam;
}

void init_io(auto* window) {
    glfwSetKeyCallback(window, on_key);
    glfwSetCursorPosCallback(window, on_mouse_moved);
    glfwSetMouseButtonCallback(window, on_mouse_pressed);
    glfwSetScrollCallback(window, on_scrolled);
    glfwSetDropCallback(window, on_dropped);
}

int main() {
    ui::init_glfw(4, 6);
    auto* window = ui::create_window(WIDTH, HEIGHT, "mesh");
    opengl::Context::instance().initialize();
    opengl::Context::instance().dump();
    init_io(window);

    const std::filesystem::path vertex_path(R"(.\vertex_shader.vert)");
    const std::filesystem::path fragment_path(R"(.\fragment_shader.frag)");
    opengl::Program program("cube");
    program.attach_shader(GL_VERTEX_SHADER,
                          opengl::utils::read_shader(vertex_path));
    program.attach_shader(GL_FRAGMENT_SHADER,
                          opengl::utils::read_shader(fragment_path));
    program.link_program();

    auto camera = create_camera();
    opengl::CameraHandler handler(camera);
    std::vector<opengl::Item> scene = read_cube();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        opengl::Context::instance().draw_background();

        program.set_mat4("view", camera.view());
        program.set_mat4("projection", camera.projection());
        for (auto& item: scene) {
            item.draw(program);
            program.set_mat4("model", item.model());
        }

        glfwSwapBuffers(window);
    }

    return EXIT_SUCCESS;
}
