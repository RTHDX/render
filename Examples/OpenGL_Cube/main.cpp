#include <glm/gtc/type_ptr.hpp>

#include <OpenGL/opengl_proc.hpp>

#include "common.hpp"
#include "ui-imgui.hpp"


glm::vec4 background = {0.5, 0.8, 0.8, 1.0};
glm::vec4 item_color = {1.0, 0.5, 0.31, 1.0};
glm::vec4 light_color = {0.8, 0.8, 1.0, 1.0};
glm::vec3 light_pos = {20.0, 20.0, 20.0 };

const std::filesystem::path vertex_path(R"(.\vertex_shader.vert)");
const std::filesystem::path fragment_path(R"(.\fragment_shader.frag)");

using Objects = std::vector<loader::Vertices>;
struct Scene {
    opengl::Camera camera;
    Objects objects;

public:
    Scene(opengl::Camera&& cam, Objects&& objects)
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

void item_color_edit() {
    IM_ASSERT(ImGui::GetCurrentContext() != NULL &&
              "Missing dear imgui context. Refer to examples app!");
    auto w_flags = ImGuiColorEditFlags_NoOptions;
    auto color = item_color;
    if (ImGui::ColorEdit4("", glm::value_ptr(color))) {
        std::cout << "Color changed" << std::endl;
        item_color = color;
    }
}

void scene_info(Scene& scene) {
    if (ImGui::CollapsingHeader("Scene")) {
        for (size_t i = 0; i < scene.objects.size(); ++i) {
            item_color_edit();
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

GLFWwindow* init(const auto& background, bool fullscreen) {
    ui::init_glfw(3, 3);
    auto* window = ui::create_window(WIDTH, HEIGHT, "mesh", fullscreen);
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


using VertexData = loader::Vertices::value_type;
int main() {
    auto* window = init(background, false);

    auto scene = create_scene();
    const size_t objects_count = scene.objects.size();
    auto program = opengl::create_program(vertex_path, fragment_path);

    std::vector<GLuint> vaos = opengl::gen_vertex_array(objects_count),
                        pos_vbo = opengl::gen_vertex_buffers(objects_count),
                        norm_vbo = opengl::gen_vertex_buffers(objects_count);

    for (size_t i = 0; i < objects_count; ++i) {
        opengl::bind_vao(vaos[i]);
        opengl::bind_vbo<VertexData>(pos_vbo[i], scene.objects[i]);
        opengl::bind_vbo<VertexData>(norm_vbo[i], scene.objects[i]);
        opengl::do_vertex_attrib_cmds(std::move(VertexData::commands()));
        opengl::bind_vao(0);
    }

    glm::mat4 model(1.0);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        opengl::Context::instance().draw_background();
        pre_process();

        opengl::use(program);
        opengl::set_vec4(program, "color", item_color);
        opengl::set_vec3(program, "light_position", light_pos);
        opengl::set_vec4(program, "light_color", light_color);
        opengl::set_mat4(program, "view", scene.camera.view());
        opengl::set_mat4(program, "projection", scene.camera.projection());
        for (size_t i = 0; i < scene.objects.size(); ++i) {
            opengl::set_mat4(program, "model", model);
            opengl::draw(opengl::DrawArrayCommand{
                .vao=vaos[i],
                .count=scene.objects[i].size()
            });
        }

        show_window(scene);
        render_imgui();

        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        opengl::Context::instance().viewport(w, h);
        scene.camera.width(w);
        scene.camera.height(h);

        glfwSwapBuffers(window);
    }

    cleanup(window);
    return EXIT_SUCCESS;
}
