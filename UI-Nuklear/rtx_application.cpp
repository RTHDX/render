#include <iostream>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nuklear.h>
#include <nuklear_glfw_gl4.h>

#include <UI/ui.hpp>
#include <OpenGL/opengl.hpp>
#include <UI/io.hpp>

#include "nuklear_widgets.hpp"
#include "rtx_application.hpp"

namespace ui::nuklear {

static void on_key(GLFWwindow*, int k, int s, int a, int m) {
    ui::io::IO::instance().emit(KeyEvent(k, s, a, m));
}

static void on_mouse_moved(GLFWwindow*, double x, double y) {
    ui::io::IO::instance().emit(MouseEvent(x, y));
}

static void on_mouse_pressed(GLFWwindow*, int b, int a, int m) {
    ui::io::IO::instance().emit(MouseButtonEvent(b, a, m));
}

static void on_scrolled(GLFWwindow*, double x, double y) {
    ui::io::IO::instance().emit(ScrollEvent(x, y));
}

static void on_dropped(GLFWwindow*, int count, const char** paths) {
    ui::io::IO::instance().emit(DropEvent(count, paths));
}

static constexpr size_t MAX_VERTEX_BUFFER = 512 * 1024;
static constexpr size_t MAX_ELEMENT_BUFFER = 128 * 1024;

static void init_fonts() {
    struct nk_font_atlas* font;
    nk_glfw3_font_stash_begin(&font);
    nk_glfw3_font_stash_end();
}

CameraMenu::CameraMenu(const rtx::Camera& camera)
    : ui::nuklear::Widget("Camera menu")
    , _camera(camera)
{}

void CameraMenu::show(struct nk_context* ctx) {
    if (nk_tree_push(ctx, NK_TREE_TAB, "Camera", NK_MINIMIZED)) {
        render_aspect_ratio(ctx);
        render_position(ctx);
        render_target(ctx);
        render_view(ctx);
        nk_tree_pop(ctx);
    }
}

void CameraMenu::render_position(struct nk_context* ctx) {
    const auto& position = _camera.position();
    std::string content = std::format(VEC3_TEMPLATE, position.x,
        position.y,
        position.z);
    if (nk_tree_push(ctx, NK_TREE_TAB, "Position", NK_MAXIMIZED)) {
        nk_label(ctx, content.data(), NK_TEXT_LEFT);
        nk_tree_pop(ctx);
    }
}

void CameraMenu::render_target(struct nk_context* ctx) {
    const auto& target = _camera.target();
    std::string content = std::format(VEC3_TEMPLATE, target.x,
        target.y,
        target.z);
    if (nk_tree_push(ctx, NK_TREE_TAB, "Center", NK_MAXIMIZED)) {
        nk_label(ctx, content.data(), NK_TEXT_LEFT);
        nk_tree_pop(ctx);
    }
}

void CameraMenu::render_view(struct nk_context* ctx) {
    const auto view = _camera.view();
    if (nk_tree_push(ctx, NK_TREE_TAB, "View", NK_MAXIMIZED)) {
        nk_layout_row_dynamic(ctx, 10, 4);
        for (size_t i = 0; i < 4; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                nk_label(ctx, std::format(FLOAT_TEMPL, view[i][j]).data(),
                    NK_TEXT_LEFT);
            }
        }
        nk_tree_pop(ctx);
    }
}

void CameraMenu::render_aspect_ratio(struct nk_context* ctx) {
    auto value = _camera.aspect_ratio();
    if (nk_tree_push(ctx, NK_TREE_TAB, "Aspect ratio", NK_MAXIMIZED)) {
        nk_label(ctx, std::format(FLOAT_TEMPL, value).data(), NK_TEXT_LEFT);
        nk_tree_pop(ctx);
    }
}

RTX_App::RTX_App(const std::string_view title, size_t width, size_t height)
    : _title(title)
    , _width(width)
    , _height(height)
{}

bool RTX_App::initialize() {
    if (!ui::init_glfw_lite()) {
        std::cout << "Unable initialize glfw" << std::endl;
        return false;
    }

    _window = ui::create_window(_width, _height, _title.data());
    opengl::Context::instance().initialize();
    opengl::Context::instance().dump();

    _ctx = nk_glfw3_init(_window, NK_GLFW3_INSTALL_CALLBACKS,
                         MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
    init_fonts();

    glfwSetKeyCallback(_window, on_key);
    glfwSetCursorPosCallback(_window, on_mouse_moved);
    glfwSetMouseButtonCallback(_window, on_mouse_pressed);
    glfwSetScrollCallback(_window, on_scrolled);
    glfwSetDropCallback(_window, on_dropped);
    return true;
}

void RTX_App::pre_render() {
    glfwPollEvents();
    nk_glfw3_new_frame();
}

void RTX_App::post_render() {
    nk_glfw3_render(NK_ANTI_ALIASING_ON);
    glfwSwapBuffers(_window);
}

}
