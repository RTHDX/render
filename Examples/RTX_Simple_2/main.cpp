#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <OpenGL/opengl_proc.hpp>
#include <UI/ui.hpp>
#include <Common/rtx_scene_default.hpp>


int main() {
    if (!ui::init_glfw_lite()) { return EXIT_FAILURE; }
    auto* window = ui::create_window(rtx::WIDTH, rtx::HEIGHT, "RTX Simple 2");
    opengl::Context::instance().initialize_light(true);

    rtx::Render<rtx::Sphere> render(std::move(rtx::make_scene()),
                                    std::move(rtx::make_camera()),
                                    rtx::Color{0.6, 0.85, 0.80});

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
