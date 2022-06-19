#include <iostream>

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
#include <nuklear.h>
#include <nuklear_glfw_gl4.h>

#include <UI/ui.hpp>
#include <UI/io.hpp>
#include <UI/observer.hpp>
#include <OpenGL/opengl.hpp>

#include "nuklear_application.hpp"


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

Application::Application(const std::string_view title, size_t w, size_t h)
    : _title(title)
    , _width(w)
    , _height(h) {
    if (!ui::init_glfw_lite()) {
        std::cout << "Unable initialize glfw" << std::endl;
        std::exit(EXIT_FAILURE);
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

Application::~Application() {
    nk_glfw3_shutdown();
    glfwTerminate();
}

void Application::run(RenderCallback& render) {
    while (!glfwWindowShouldClose(_window)) {
        glfwPollEvents();
        nk_glfw3_new_frame();

        if (render) {
            render();
        } else {
            opengl::Context::instance().draw_background();
        }
        for (uWidget& widget : _widgets) { widget->show(); }

        nk_glfw3_render(NK_ANTI_ALIASING_ON);
        glfwSwapBuffers(_window);
    }
}

void Application::append(uWidget&& widget) {
    _widgets.push_back(std::move(widget));
}

}
