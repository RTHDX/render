#include <iostream>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nuklear.h>
#include <nuklear_glfw_gl4.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <OpenGL/opengl.hpp>
#include <UI/ui.hpp>
#include <UI/property.hpp>
#include <RTX/rtx.hpp>
#include <UI-Nuklear/nuklear_application.hpp>
#include <UI-Nuklear/nuklear_widgets.hpp>

#include <Common/rtx_scene_default.hpp>


class Label final : public ui::nuklear::Widget {
public:
    Label(const rtx::Camera& camera, nk_context* ctx)
        : ui::nuklear::Widget("Camera position", ctx)
        , _camera(camera)
    {}

    void show() override {
        static constexpr char TEMPLATE[] = "Position({};{};{})";

        const rtx::Point& point = _camera.position();
        std::string content = std::format(TEMPLATE, point.x, point.y, point.z);
        nk_label(ctx(), content.data(), NK_TEXT_LEFT);
    }

private:
    const rtx::Camera& _camera;
};


class CameraMenu final : public ui::nuklear::Widget {
public:
    CameraMenu(const rtx::Camera& camera, nk_context* ctx)
        : ui::nuklear::Widget("Camera menu", ctx)
        , _camera(camera)
    {}

    void show() {
        if (nk_tree_push(ctx(), NK_TREE_TAB, "Window", NK_MINIMIZED)) {
            nk_layout_row_dynamic(ctx(), 20, 1);
            for (const auto& child : children()) { child->show(); }
        } nk_tree_pop(ctx());
    }

private:
    const rtx::Camera& _camera;
};


class CameraBox : public ui::nuklear::Window {
public:
    CameraBox(rtx::Camera& camera, nk_context* ctx)
        : ui::nuklear::Window("Camera BOX", ctx,
                              nk_rect(0, 0, 200, rtx::HEIGHT),
                              NK_WINDOW_MINIMIZABLE | NK_WINDOW_NO_SCROLLBAR)
        , _camera(camera)
    {
        ui::nuklear::uWidget menu(std::make_unique<CameraMenu>(camera, ctx));
        menu->append(std::make_unique<Label>(camera, ctx));
        append(std::move(menu));
    }

private:
    rtx::Camera& _camera;
};

int main() {
    ui::nuklear::Application app("movable camera", rtx::WIDTH, rtx::HEIGHT);
    
    auto& camera = rtx::make_camera();
    //rtx::Render render(rtx::make_scene(), {0.8, 0.8, 0.8}, camera);
    rtx::MultiThreadRender render(rtx::make_scene(), {0.8, 0.8, 0.8}, camera);
    ui::nuklear::RenderCallback callback = [&render]() {
        //BENCHMARK(render.render());
        render.render();
        const auto& buffer = render.buffer();
        glDrawPixels(rtx::WIDTH, rtx::HEIGHT, GL_RGB ,GL_FLOAT,
                     glm::value_ptr(*buffer.data()));
    };

    //app.append(std::make_unique<CameraBox>(camera, app.ctx()));
    app.run(callback);
    return EXIT_SUCCESS;
}
