#include <iostream>
#include <glad/glad.h>

#include <OpenGL/opengl_proc.hpp>

#include "imgui_widget_render.hpp"


namespace ui {

static ImVec2 convert(const glm::vec2& vec);

void ImGuiWidgetRender::visit(Window& widget) {
    ImVec2 parent_size = ImGui::GetMainViewport()->Size;
    ImVec2 pos = absolute_vec2(parent_size, convert(widget.top_left()));
    ImVec2 size = absolute_vec2(parent_size, convert(widget.size()));

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    const auto* lbl = widget.title().data();
    auto flags = widget.flags();
    bool is_visible = widget.is_visible();
    ImGui::Begin(lbl, &is_visible, flags);
    for (auto& elem : widget.components()) {
        elem->accept(*this);
    }
    widget.is_visible(is_visible);
    ImGui::End();
}

void ImGuiWidgetRender::visit(Canvas& widget) {
    ImVec2 parent_size = ImGui::GetWindowSize();
    ImVec2 size = absolute_vec2(parent_size, convert(widget.size()));
    ImVec2 s_size = screen_size();

    widget.update(size.x, size.y);
    const auto& fbuff = widget.fbuff();
    const auto& back = widget.background();
    opengl::bind_fbo(fbuff.fbo);
    opengl::background(back, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    opengl::viewport(0, 0, size.x, size.y);
    for (const auto& entity : widget.entities()) {
        const auto& render_data = entity->render_data();
        auto program = render_data.program;
        opengl::use(program);
        opengl::set_mat4(program, "projection", widget.projection());
        opengl::set_mat4(program, "view", widget.view());
        opengl::set_mat4(program, "model", entity->model());
        opengl::bind_vao(render_data.vao);
        opengl::draw(entity->draw_command());
        opengl::bind_vao(0);
        opengl::use(0);
    }
    opengl::viewport(0, 0, s_size.x, s_size.y);
    opengl::bind_fbo(0);

    ImGui::BeginChild(widget.title().data(), size);

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tints
    ImGui::GetWindowDrawList()->AddImage(
        (void*)(intptr_t)fbuff.texture.id,
        pos,
        ImVec2(pos.x + size.x, pos.y + size.y),
        uv_min, uv_max,
        ImGui::ColorConvertFloat4ToU32(tint_col)
    );

    ImGui::EndChild();
}

ImVec2 ImGuiWidgetRender::absolute_vec2(const ImVec2& parent_size,
                                        const ImVec2& rel_vec) const {
    return {
        parent_size.x * (rel_vec.x / 100.0f),
        parent_size.y * (rel_vec.y / 100.0f)
    };
}

ImVec2 ImGuiWidgetRender::screen_size() const {
    return ImGui::GetMainViewport()->Size;
}

static ImVec2 convert(const glm::vec2& vec) {
    return {vec.x, vec.y};
}

}