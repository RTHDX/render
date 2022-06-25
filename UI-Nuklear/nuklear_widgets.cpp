#include <iostream>
#include <format>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL4_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
//#include <nuklear.h>
//#include <nuklear_glfw_gl4.h>

#include "nuklear_widgets.hpp"
#include <nuklear_glfw_gl4.h>


namespace ui::nuklear {

Widget::Widget(const std::string_view name)
    : _name(name)
{}

void Widget::append(uWidget&& child) {
    _children.push_back(std::move(child));
}


Window::Window(const std::string_view name,
               const struct nk_rect rectangle, nk_flags flags)
    : Widget(name)
    , _dim(rectangle)
    , _flags(flags)
{}

void Window::show(struct nk_context* ctx) {
    if (nk_begin(ctx, name().data(), _dim, _flags)) {
        for (auto& child: children()) {
            child->show(ctx);
        }
    } nk_end(ctx);
}

}
