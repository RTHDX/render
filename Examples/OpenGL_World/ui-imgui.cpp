#include <format>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

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


Context::Context(Scene& scene)
    : _scene(scene)
{}

void Context::show_main_window() {
    if (_scene.is_any_item_active() &&
        _last_active_item != _scene.active_item()) {
        _last_active_item = _scene.active_item();
        fill_vertices(_last_active_item->vertices());
    }

    ImGui::Begin("window");
    ImGui::SetWindowPos(ImVec2(0.0, 0.0));
    show_mat4_table(_scene.camera().projection(), "Projection");
    show_mat4_table(_scene.camera().view(), "View");
    if (_last_active_item) {
        const auto& model = _last_active_item->model();
        show_mat4_table(model, "Model");

        auto pvm = _scene.camera().projection() *
            _scene.camera().view() *
            _last_active_item->model();
        show_mat4_table(pvm, "Projection * View * Model");

        show_vertices(_last_vertices);
        pvm_vertices(_last_vertices, model);

        edit_centroid();
    }
    ImGui::End();
}

void Context::show_mat4_table(const glm::mat4& mat, const char* id) const {
    if (!ImGui::CollapsingHeader(id)) { return ; }
    if (!ImGui::BeginTable(id, 4, MAT4)) { return; }

    for (int i = 0; i < 4; ++i) {
        ImGui::TableNextRow();
        for (int j = 0; j < 4; ++j) {
            ImGui::TableSetColumnIndex(j);
            auto string = std::format("{:.3}", mat[i][j]);
            ImGui::TextUnformatted(string.c_str());
        }
    }
    ImGui::EndTable();
}

void Context::show_vertices(const flaten_vertices_t& vertices) const {
    if (!ImGui::CollapsingHeader("Vertices")) { return; }
    if (!ImGui::BeginTable("Vertices", 3, VERT)) { return; }

    ImGui::TableSetupColumn("x");
    ImGui::TableSetupColumn("y");
    ImGui::TableSetupColumn("z");
    ImGui::TableHeadersRow();
    for (const opengl::vec3pos_vec3norm_t& vertex : vertices) {
        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        float x = vertex.pos.x;
        ImGui::TextUnformatted(std::format("{:.3}", x).c_str());
    
        ImGui::TableSetColumnIndex(1);
        float y = vertex.pos.y;
        ImGui::TextUnformatted(std::format("{:.3}", y).c_str());

        ImGui::TableSetColumnIndex(2);
        float z = vertex.pos.z;
        ImGui::TextUnformatted(std::format("{:.3}", z).c_str());
    }

    ImGui::EndTable();
}

void Context::pvm_vertices(const flaten_vertices_t& vertices,
                           const glm::mat4& pvm) const {
    if (!ImGui::CollapsingHeader("PMV * Vertices")) { return; }
    if (!ImGui::BeginTable("pmv_vert", 3, VERT)) { return; }

    ImGui::TableSetupColumn("x");
    ImGui::TableSetupColumn("y");
    ImGui::TableSetupColumn("z");
    ImGui::TableHeadersRow();
    for (const auto& vertex : vertices) {
        ImGui::TableNextRow();

        glm::vec4 result = pvm * glm::vec4(vertex.pos, 1.0);
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted(std::format("{:.3}", result.x).c_str());

        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted(std::format("{:.3}", result.y).c_str());

        ImGui::TableSetColumnIndex(2);
        ImGui::TextUnformatted(std::format("{:.3}", result.z).c_str());
    }

    ImGui::EndTable();
}

void Context::fill_vertices(const vertices_t& vert) {
    for (const loader::Vertices& mesh : vert) {
        for (const opengl::vec3pos_vec3norm_t& vertex : mesh) {
            auto begin = std::cbegin(_last_vertices);
            auto end = std::cend(_last_vertices);
            const bool is_contains = std::find_if(begin, end,
                [vertex](const opengl::vec3pos_vec3norm_t& item) {
                    return item.pos == vertex.pos;
                }
            ) == end;
            if (!is_contains) { continue; }

            _last_vertices.push_back(vertex);
        }
    }
}

void Context::edit_centroid() {
    //if (!ImGui::CollapsingHeader("Move item")) { return; }
    static constexpr float STEP = 0.1;

    const auto& model = _last_active_item->model();
    _centroid = {model[3][0], model[3][1], model[3][2]};
    glm::vec3 dxyz = {0.0, 0.0, 0.0};
    if (ImGui::InputScalar("x ", ImGuiDataType_Float, &_centroid.x, &STEP)) {
        dxyz.x = -(model[3][0] - _centroid.x);
    }
    if (ImGui::InputScalar("y ", ImGuiDataType_Float, &_centroid.y, &STEP)) {
        dxyz.y = -(model[3][1] - _centroid.y);
    }
    if (ImGui::InputScalar("z ", ImGuiDataType_Float, &_centroid.z, &STEP)) {
        dxyz.z = -(model[3][2] - _centroid.z);
    }

    _last_active_item->modify(glm::translate(model, dxyz));

}

}
