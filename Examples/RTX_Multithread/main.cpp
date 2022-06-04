#include "scene.hpp"

int main() {
    if (!ui::init_glfw_lite()) { return EXIT_FAILURE; }
    auto* window = ui::create_window(WIDTH, HEIGHT, "RTX Simple 2");
    glfwSetFramebufferSizeCallback(window, opengl::framebuffer_size_callback);

    opengl::Context::instance().initialize();
    opengl::Context::instance().dump();

    rtx::MultiThreadRender render(make_scene(), rtx::Color{0.6, 0.85, 0.80},
                                  make_camera());

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        BENCHMARK(render.render());
        const auto& framebuffer = render.buffer();
        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_FLOAT,
            glm::value_ptr(*framebuffer.data()));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
