#pragma once

#include <functional>

#include <OpenGL/camera.hpp>

#include "base_application.hpp"


namespace ui::nuklear {
using RenderCallback = std::function<void(void)>;

class OpenGL_App final : public BApplication {
public:
    OpenGL_App(const std::string_view title, size_t w, size_t h,
               opengl::Camera&& camera);

    void run(const RenderCallback& callback);
    const opengl::Camera& camera() const { return _camera; }

protected:
    bool custom_initialize() override;

private:
    opengl::Camera _camera;
    opengl::CameraHandler _handler;
};

}
