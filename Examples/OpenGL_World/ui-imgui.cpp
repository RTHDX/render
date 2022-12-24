#include <format>

#include "ui-imgui.hpp"


namespace ui::imgui {

ImGuiIO& init_imgui(GLFWwindow* window, const char* glsl_version) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return io;
}

void cleanup(GLFWwindow* window) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void pre_process() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void render_imgui() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

static void show_mat4_table(const glm::mat4& model, const char* id);

void show_main_widget(Scene& scene) {
    ImGui::Begin("window");
    ImGui::SetWindowPos(ImVec2(0.0, 0.0));
    show_mat4_table(scene.camera().projection(), "Projection");
    show_mat4_table(scene.camera().view(), "View");
    if (scene.is_any_item_active()) {
        const auto* active_item = scene.active_item();
        assert(active_item != nullptr);
        show_mat4_table(active_item->model(), "Model");

        auto pvm = scene.camera().projection() *
                   scene.camera().view() *
                   active_item->model();
        show_mat4_table(pvm, "Projection View Model");
    }
    ImGui::End();
}

static void show_mat4_table(const glm::mat4& model, const char* id) {
    static ImGuiTableFlags flags = ImGuiTableFlags_Borders
                                   | ImGuiTableFlags_RowBg;
    ImGui::Selectable(id);
    if (ImGui::BeginTable(id, 4, flags)) {
        for (ptrdiff_t i = 0; i < 4; ++i) {
            ImGui::TableNextRow();
            for (ptrdiff_t j = 0; j < 4; ++j) {
                ImGui::TableSetColumnIndex(j);
                auto string = std::format("{:.3}", model[i][j]);
                ImGui::TextUnformatted(string.c_str());
            }
        }
        ImGui::EndTable();
    }
}

}
