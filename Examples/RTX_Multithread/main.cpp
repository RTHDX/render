#include <glm/gtc/type_ptr.hpp>
#include <UI/ui.hpp>
#include <Common/rtx_scene_default.hpp>

int main() {
    if (!ui::init_glfw_lite()) { return EXIT_FAILURE; }
    auto* window = ui::create_window(rtx::WIDTH, rtx::HEIGHT, "RTX Simple 2");
    glfwSetFramebufferSizeCallback(window, opengl::framebuffer_size_callback);

    opengl::Context::instance().initialize();
    opengl::Context::instance().dump();

    rtx::MultiThreadRender render(rtx::make_scene(), rtx::Color{0.01, 0.05, 0.08},
                                  rtx::make_camera());

    while (!glfwWindowShouldClose(window)) {
        //process_input(window);

        BENCHMARK(render.render());
        const auto& framebuffer = render.buffer();
        glDrawPixels(rtx::WIDTH, rtx::HEIGHT, GL_RGB, GL_FLOAT,
            glm::value_ptr(*framebuffer.data()));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
