#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <nuklear.h>
#include <nuklear_glfw_gl4.h>

#include "nuklear_widgets.hpp"


namespace ui::nuklear {

Widget::Widget(const std::string_view name, nk_context* ctx)
    : _name(name)
    , _ctx(ctx)
{}

void Widget::append(uWidget&& child) {
    _children.push_back(std::move(child));
}


Window::Window(const std::string_view name, struct nk_context* ctx,
               const struct nk_rect rectangle, nk_flags flags)
    : Widget(name, ctx)
    , _dim(rectangle)
    , _flags(flags)
{}

void Window::show() {
    if (nk_begin(ctx(), name().data(), _dim, _flags)) {
        for (auto& child: children()) {
            //nk_layout_row_static(ctx(), _dim.h / children().size(), _dim.w, 1);
            child->show();
        }
    } nk_end(ctx());
}


LabelButton::LabelButton(const std::string_view name, struct nk_context* ctx)
    : Widget(name, ctx)
{}

void LabelButton::show() {
    if (nk_button_label(ctx(), name().data())) {
        ;
    }
}

}
