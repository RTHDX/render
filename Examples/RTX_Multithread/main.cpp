#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <OpenGL/opengl_proc.hpp>
#include <UI/ui.hpp>
#include <Common/rtx_scene_default.hpp>

int main() {
    if (!ui::init_glfw_lite()) { return EXIT_FAILURE; }
    auto* window = ui::create_window(rtx::WIDTH, rtx::HEIGHT, "RTX Simple 2");
    //glfwSetFramebufferSizeCallback(window, opengl::framebuffer_size_callback);

    opengl::Context::instance().initialize();
    opengl::Context::instance().dump();

    rtx::MultiThreadRender render(rtx::make_scene(), rtx::make_camera(),
                                  {0.01, 0.05, 0.08});

    while (!glfwWindowShouldClose(window)) {
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
