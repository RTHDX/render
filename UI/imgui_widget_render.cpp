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
    ImGui::Dummy(size);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    glm::vec4 c (1.0, 0.0, 0.0, 1.0);
    ImVec2 p = ImGui::GetItemRectMin();
    ImVec2 rect_min = ImVec2(p.x, p.y);
    ImVec2 rect_max = ImVec2(p.x + ImGui::GetItemRectSize().x,
                             p.y + ImGui::GetItemRectSize().y);
    draw_list->AddRectFilled(rect_min, rect_max, ImColor(c.r, c.g, c.b, c.a));
}

ImVec2 ImGuiWidgetRender::absolute_vec2(const ImVec2& parent_size,
                                        const ImVec2& rel_vec) const {
    return {
        parent_size.x * (rel_vec.x / 100.0f),
        parent_size.y * (rel_vec.y / 100.0f)
    };
}

static ImVec2 convert(const glm::vec2& vec) {
    return {vec.x, vec.y};
}

}