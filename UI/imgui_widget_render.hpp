#pragma once

#include <imgui.h>

#include "widgets.hpp"


namespace ui {

class ImGuiWidgetRender final : public Visitor {
public:
    void visit(Window& widget) override;
    void visit(Canvas& widget) override;

private:
    ImVec2 absolute_vec2(const ImVec2& parent_vec, const ImVec2& rel_vec) const;
};

}
