#include <UI/ui.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <GLFW/glfw3.h>

#include "opengl_application.hpp"

namespace ui::nuklear {

OpenGL_App::OpenGL_App(const std::string_view title, size_t w, size_t h,
                       opengl::Camera&& camera)
    : BApplication(title, w, h)
    , _camera(std::move(camera))
    , _handler(_camera)
{}

bool OpenGL_App::custom_initialize() { return init_glfw(4, 6); }

void OpenGL_App::run(const RenderCallback& callback) {
    opengl::Context::instance().background({0.2f, 0.5f, 0.5f, 1.0f});

    while (!glfwWindowShouldClose(window())) {
        pre_render();

        opengl::Context::instance().draw_background();
        for (auto& widget : _widgets) { widget->show(ctx()); }
        if (callback) { callback(); }

        post_render();
    }
}

}
