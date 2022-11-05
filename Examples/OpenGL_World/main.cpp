#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <UI/ui.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/camera.hpp>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;


struct Item {
    glm::mat4 model;
    
};


int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }

    auto* window = ui::create_window(WIDTH, HEIGHT, "world");
    opengl::Camera camera(WIDTH, HEIGHT, glm::radians(45.0), {10, 10, 10});

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
