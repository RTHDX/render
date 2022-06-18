#include <iostream>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nuklear.h>
#include <nuklear_glfw_gl4.h>
#include <glm/glm.hpp>

#include <OpenGL/opengl.hpp>
#include <UI/ui.hpp>
#include <UI/property.hpp>
#include <RTX/rtx.hpp>

#include <UI-Nuklear/nuklear_application.hpp>
#include <UI-Nuklear/nuklear_widgets.hpp>


class Label final : public ui::nuklear::Widget {
public:
    Label(ui::Property<rtx::Point>& prop, nk_context* ctx)
        : ui::nuklear::Widget("Camera position", ctx)
        , _position(prop)
    {}

    void show() override {
        static constexpr char TEMPLATE[] = "Position({};{};{})";

        const rtx::Point& point = _position.get();
        std::string content = std::format(TEMPLATE, point.x, point.y, point.z);
        nk_label(ctx(), content.data(), NK_TEXT_LEFT);
    }

private:
    ui::Property<rtx::Point>& _position;
};


class CameraBox : public ui::nuklear::Window {
public:
    CameraBox(rtx::Camera& camera, nk_context* ctx)
        : ui::nuklear::Window("camera box", ctx,
                              nk_rect(0, 0, 200, 100),
                              NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE)
        , _camera(camera)
    {}

private:
    rtx::Camera& _camera;
};

int main() {
    const size_t width = 800, height = 600;
    ui::nuklear::Application app("movable cubes");
    
    rtx::Camera camera({20.0f, 20.0f, 20.0f},
                       glm::radians(90.0f), width, height);
    app.append(std::make_unique<CameraBox>(camera, app.ctx()));
    app.run();
    return EXIT_SUCCESS;
}
