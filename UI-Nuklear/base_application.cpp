#include <iostream>

#include <UI/ui.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <UI/io.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nuklear.h>
#include <nuklear_glfw_gl4.h>

#include "base_application.hpp"


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

BApplication::BApplication(const std::string_view title, size_t width,
                           size_t height)
    : _title(title)
    , _width(width)
    , _height(height)
{}

BApplication::~BApplication() {
    glfwTerminate();
}

void BApplication::initialize() {
    if (!custom_initialize()) {
        std::cerr << "Custom initialization failed" << std::endl;
        return;
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
}

void BApplication::pre_render() {
    glfwPollEvents();
    nk_glfw3_new_frame();
}

void BApplication::post_render() {
    nk_glfw3_render(NK_ANTI_ALIASING_ON);
    glfwSwapBuffers(_window);
}

}
