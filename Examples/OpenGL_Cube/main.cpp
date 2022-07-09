#include <glm/gtc/type_ptr.hpp>

#include <OpenGL/operators.hpp>

#include "common.hpp"
#include "ui-imgui.hpp"


glm::vec4 background = {0.5, 0.8, 0.8, 1.0};
glm::vec4 item_color = {1.0, 0.5, 0.31, 1.0};
glm::vec4 light_color = {0.8, 0.8, 1.0, 1.0};
glm::vec3 light_pos = {20.0, 20.0, 20.0 };

const std::filesystem::path vertex_path(R"(.\vertex_shader.vert)");
const std::filesystem::path fragment_path(R"(.\fragment_shader.frag)");


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
    IM_ASSERT(ImGui::GetCurrentContext() != NULL &&
              "Missing dear imgui context. Refer to examples app!");
    auto& color = opengl::Context::instance().background();
    if (ImGui::CollapsingHeader("Background")) {
        if (ImGui::ColorEdit4("", glm::value_ptr(color))) {
            opengl::Context::instance().background(color);
        }
    }
}

void item_info() {
    IM_ASSERT(ImGui::GetCurrentContext() != NULL &&
              "Missing dear imgui context. Refer to examples app!");
    auto w_flags = ImGuiColorEditFlags_NoOptions;
    auto color = item_color;
    if (ImGui::ColorEdit4("", glm::value_ptr(color))) {
        std::cout << "Color changed" << std::endl;
        item_color = color;
    }
}

void item_operators(opengl::Item& item) {
    static bool rotate = false;
    if (rotate) {
        static opengl::Rotate rotor(glm::radians(5.0f));
        rotor.accept(item);
    } ImGui::Checkbox("Rotation", &rotate);

    static bool move_forward = false;
    if (move_forward) {
        static opengl::Move forward(opengl::Move::Direction::FORWARD, .01);
        forward.accept(item);
    } ImGui::Checkbox("Move foward", &move_forward);

    static bool move_back = false;
    if (move_back) {
        static opengl::Move back(opengl::Move::Direction::BACKWARD, 0.01);
        back.accept(item);
    } ImGui::Checkbox("Move back", &move_back);
}

void scene_info(Scene& scene) {
    if (ImGui::CollapsingHeader("Scene")) {
        for (size_t i = 0; i < scene.objects.size(); ++i) {
            //item_info();
            item_operators(scene.objects[i]);
        }
    }
}

void show_window(Scene& scene) {
    ImGui::SetWindowPos(ImVec2(0.0, 0.0));
    ImGui::Begin("window");
    ImGui::SetWindowSize(ImVec2(200.0, 400.0));
    background_edit();
    scene_info(scene);
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
    auto program = opengl::create_program(vertex_path, fragment_path);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        opengl::Context::instance().draw_background();
        pre_process();

        glUseProgram(program);

        opengl::set_vec4(program, "color", item_color);
        opengl::set_vec3(program, "light_position", light_pos);
        opengl::set_vec4(program, "light_color", light_color);
        opengl::set_mat4(program, "view", scene.camera.view());
        opengl::set_mat4(program, "projection", scene.camera.projection());
        for (auto& item: scene.objects) {
            item.draw();
            opengl::set_mat4(program, "model", item.model());
        }

        show_window(scene);

        render_imgui();

        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glfwSwapBuffers(window);
        scene.camera.width(w);
        scene.camera.height(h);
    }

    cleanup(window);
    return EXIT_SUCCESS;
}
