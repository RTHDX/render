#include <glm/gtc/type_ptr.hpp>

#include "common.hpp"
#include "ui-imgui.hpp"

void background_edit(glm::vec4& color) {
    //IM_ASSERT(ImGui::GetCurrentContext() != nullptr &&
    //          "Missing dear imgui context!");

    auto w_flags = ImGuiColorEditFlags_NoLabel |
                   ImGuiColorEditFlags_NoSidePreview |
        ImGuiColorEditFlags_NoOptions;

    if (ImGui::CollapsingHeader("Background")) {
        if (ImGui::ColorEdit4("", glm::value_ptr(color), w_flags)) {
            opengl::Context::instance().background(color);
        }
    }
}

void scene_info(const std::vector<opengl::Item>& items) {
    if (ImGui::CollapsingHeader("Scene")) {
        for (size_t i = 0; i < 5; ++i) {
            ImGui::BulletText("Text");
        }
    }
}

void show_window(glm::vec4& color) {
    ImGui::SetWindowPos(ImVec2(0.0, 0.0));
    ImGui::Begin("window");
    ImGui::SetWindowSize(ImVec2(200.0, 400.0));
    background_edit(color);
    scene_info({});
    ImGui::End();
}



int main() {
    glm::vec4 background = {0.5, 0.8, 0.8, 1.0};
    ui::init_glfw(4, 6);
    auto* window = ui::create_window(WIDTH, HEIGHT, "mesh");
    opengl::Context::instance().initialize();
    opengl::Context::instance().dump();
    opengl::Context::instance().background(background);
    init_io(window);
    auto& io_imgui = init_imgui(window, "#version 460");

    auto program = create_program();

    auto camera = create_camera();
    opengl::CameraHandler handler(camera);
    std::vector<opengl::Item> scene = read_cube();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        opengl::Context::instance().draw_background();
        pre_process();

        program.set_mat4("view", camera.view());
        program.set_mat4("projection", camera.projection());
        for (auto& item: scene) {
            item.draw(program);
            program.set_mat4("model", item.model());
        }

        show_window(background);

        render_imgui();

        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glfwSwapBuffers(window);
    }

    cleanup(window);
    return EXIT_SUCCESS;
}
