#include "common.hpp"
#include "ui-imgui.hpp"


int main() {
    ui::init_glfw(4, 6);
    auto* window = ui::create_window(WIDTH, HEIGHT, "mesh");
    opengl::Context::instance().initialize();
    opengl::Context::instance().dump();
    init_io(window);
    auto& io_imgui = init_imgui(window, "#version 460");

    auto program = create_program();

    auto camera = create_camera();
    opengl::CameraHandler handler(camera);
    std::vector<opengl::Item> scene = read_cube();

    while (!glfwWindowShouldClose(window)) {
        pre_process();
        ImGui::Begin("Hello window");
        ImGui::End();
        render_imgui();

        program.set_mat4("view", camera.view());
        program.set_mat4("projection", camera.projection());
        for (auto& item: scene) {
            item.draw(program);
            program.set_mat4("model", item.model());
        }

        glfwSwapBuffers(window);
    }

    cleanup(window);
    return EXIT_SUCCESS;
}
