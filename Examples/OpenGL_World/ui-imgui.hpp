#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "item.hpp"

namespace ui::imgui {

ImGuiIO& init_imgui(GLFWwindow* window, const char* glsl_version);
void cleanup(GLFWwindow* window);
void pre_process();
void render_imgui();

struct Context {
    using vertices_t = std::vector<loader::Vertices>;
    using flaten_vertices_t = std::vector<vertices_t::value_type::value_type>;

    static constexpr ImGuiTableFlags MAT4 = ImGuiTableFlags_Borders
                                            | ImGuiTableFlags_RowBg;

    static constexpr ImGuiTableFlags VERT = ImGuiTableFlags_Borders
                                            | ImGuiTableFlags_RowBg;

public:
    explicit Context(Scene& scene);

    void show_main_window();

private:
    void show_mat4_table(const glm::mat4& mat, const char* id) const;
    void show_vertices(const flaten_vertices_t& vertices) const;
    void pvm_vertices(const flaten_vertices_t& vertices, const glm::mat4& mat) const;
    void fill_vertices(const vertices_t& v);
    void edit_centroid();

private:
    Scene& _scene;
    Item3D* _last_active_item = nullptr;
    flaten_vertices_t _last_vertices;
    glm::vec3 _centroid;
};

}
