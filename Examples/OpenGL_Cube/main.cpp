#include "common.hpp"
#include "ui-nuk.hpp"


int main() {
    ui::init_glfw(4, 6);
    auto* window = ui::create_window(WIDTH, HEIGHT, "mesh");
    opengl::Context::instance().initialize();
    opengl::Context::instance().dump();
    init_io(window);
    auto* ctx = init_context(window);

    auto program = create_program();

    auto camera = create_camera();
    opengl::CameraHandler handler(camera);
    std::vector<opengl::Item> scene = read_cube();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        nk_glfw3_new_frame();

        opengl::Context::instance().draw_background();

        program.set_mat4("view", camera.view());
        program.set_mat4("projection", camera.projection());
        for (auto& item: scene) {
            item.draw(program);
            program.set_mat4("model", item.model());
        }

        //nk_glfw3_render(NK_ANTI_ALIASING_ON);
        glfwSwapBuffers(window);
    }

    return EXIT_SUCCESS;
}
