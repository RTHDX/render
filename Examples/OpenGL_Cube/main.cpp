#include <glm/gtc/type_ptr.hpp>

#include "common.hpp"
#include "ui-imgui.hpp"


glm::vec4 background = { 0.5, 0.8, 0.8, 1.0 };
glm::vec4 item_color = { 1.0, 0.5, 0.31, 1.0 };

glm::vec4 light_color = { 0.8, 0.8, 1.0, 1.0 };
glm::vec3 light_pos = { 20.0, 20.0, 20.0 };


struct Scene {
    opengl::Camera camera;
    std::vector<opengl::Item> objects;

public:
    Scene(opengl::Camera&& cam, std::vector<opengl::Item>&& objects)
        : camera(std::move(cam))
        , objects(std::move(objects))
        , handler(camera)
    {}

private:
    opengl::CameraHandler handler;
};

struct UI {
    bool background_opened = false;
    bool items_visible     = false;
};

static UI ui_state;


void background_edit() {
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");
    auto& color = opengl::Context::instance().background();
    if (ImGui::CollapsingHeader("Background")) {
        if (ImGui::ColorEdit4("", glm::value_ptr(color))) {
            opengl::Context::instance().background(color);
        }
    }
}

void item_info() {
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");
    auto w_flags = ImGuiColorEditFlags_NoOptions;
    auto color = item_color;
    if (ImGui::ColorEdit4("", glm::value_ptr(color))) {
        std::cout << "Color changed" << std::endl;
        item_color = color;
    }
}

void scene_info(const std::vector<opengl::Item>& items) {
    if (ImGui::CollapsingHeader("Scene")) {
        for (size_t i = 0; i < items.size(); ++i) {
            item_info();
        }
    }
}

void show_window(const Scene& scene) {
    ImGui::SetWindowPos(ImVec2(0.0, 0.0));
    ImGui::Begin("window");
    ImGui::SetWindowSize(ImVec2(200.0, 400.0));
    background_edit();
    scene_info(scene.objects);
    ImGui::End();
}

GLFWwindow* init(const auto& background) {
    ui::init_glfw(4, 6);
    auto* window = ui::create_window(WIDTH, HEIGHT, "mesh");
    opengl::Context::instance().initialize();
    opengl::Context::instance().dump();
    opengl::Context::instance().background(background);
    init_io(window);
    init_imgui(window, "#version 460");
    return window;
}

auto create_scene() {
    return Scene {
        std::move(create_camera()),
        std::move(read_cube())
    };
}



int main() {
    auto* window = init(background);

    auto scene = create_scene();
    auto program = create_program();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        opengl::Context::instance().draw_background();
        pre_process();

        program.set_vec4("color", item_color);
        program.set_vec3("light_position", light_pos);
        program.set_vec4("light_color", light_color);
        program.set_mat4("view", scene.camera.view());
        program.set_mat4("projection", scene.camera.projection());
        for (auto& item: scene.objects) {
            item.draw(program);
            program.set_mat4("model", item.model());
        }

        show_window(scene);

        render_imgui();

        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glfwSwapBuffers(window);
    }

    cleanup(window);
    return EXIT_SUCCESS;
}
