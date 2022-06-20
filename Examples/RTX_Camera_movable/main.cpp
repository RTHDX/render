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


class CameraMenu final : public ui::nuklear::Widget {
    static constexpr char FLOAT_TEMPL[] = "{:.4f}";
    static constexpr char VEC3_TEMPLATE[] = "{:.4f} | {:.4f} | {:.4f}";
    static constexpr char MAT4_TEMPL[] =
        "({:.4f};{:.4f};{:.4f};{:.4f}"
        " {:.4f};{:.4f};{:.4f};{:.4f}"
        " {:.4f};{:.4f};{:.4f};{:.4f}"
        " {:.4f};{:.4f};{:.4f};{:.4f})";

public:
    CameraMenu(const rtx::Camera& camera, nk_context* ctx)
        : ui::nuklear::Widget("Camera menu", ctx)
        , _camera(camera)
    {}

    void show() {
        if (nk_tree_push(ctx(), NK_TREE_TAB, "Camera", NK_MINIMIZED)) {
            render_aspect_ratio();
            render_position();
            render_target();
            render_view();
            nk_tree_pop(ctx());
        }
    }

private:
    void render_position() {
        const auto& position = _camera.position();
        std::string content = std::format(VEC3_TEMPLATE, position.x,
                                                         position.y,
                                                         position.z);
        if (nk_tree_push(ctx(), NK_TREE_TAB, "Position", NK_MAXIMIZED)) {
            nk_label(ctx(), content.data(), NK_TEXT_LEFT);
            nk_tree_pop(ctx());
        }
    }

    void render_target() {
        const auto& target = _camera.target();
        std::string content = std::format(VEC3_TEMPLATE, target.x,
                                                         target.y,
                                                         target.z);
        if (nk_tree_push(ctx(), NK_TREE_TAB, "Center", NK_MAXIMIZED)) {
            nk_label(ctx(), content.data(), NK_TEXT_LEFT);
            nk_tree_pop(ctx());
        }
    }

    void render_view() {
        const auto view = _camera.view();
        if (nk_tree_push(ctx(), NK_TREE_TAB, "View", NK_MAXIMIZED)) {
            nk_layout_row_dynamic(ctx(), 10, 4);
            for (size_t i = 0; i < 4; ++i) {
                for (size_t j = 0; j < 4; ++j) {
                    nk_label(ctx(), std::format(FLOAT_TEMPL, view[i][j]).data(),
                             NK_TEXT_LEFT);
                }
            }
            nk_tree_pop(ctx());
        }
    }

    void render_aspect_ratio() {
        auto value = _camera.aspect_ratio();
        if (nk_tree_push(ctx(), NK_TREE_TAB, "Aspect ratio", NK_MAXIMIZED)) {
            nk_label(ctx(), std::format(FLOAT_TEMPL, value).data(), NK_TEXT_LEFT);
            nk_tree_pop(ctx());
        }
    }

private:
    const rtx::Camera& _camera;
};


class CameraBox : public ui::nuklear::Window {
public:
    CameraBox(const rtx::Camera& camera, nk_context* ctx)
        : ui::nuklear::Window("Camera BOX", ctx,
                              nk_rect(0, 0, 300, rtx::HEIGHT),
                              NK_WINDOW_MINIMIZABLE | NK_WINDOW_NO_SCROLLBAR)
        , _camera(camera)
    {
        ui::nuklear::uWidget menu(std::make_unique<CameraMenu>(camera, ctx));
        append(std::move(menu));
    }

private:
    const rtx::Camera& _camera;
};

int main() {
    ui::nuklear::Application app("movable camera", rtx::WIDTH, rtx::HEIGHT);

    rtx::MultiThreadRender render(rtx::make_scene(), {0.8, 0.8, 0.8},
                                  rtx::make_camera());
    ui::nuklear::RenderCallback callback = [&render]() {
        BENCHMARK(render.render());
        const auto& buffer = render.buffer();
        glDrawPixels(rtx::WIDTH, rtx::HEIGHT, GL_RGB ,GL_FLOAT,
                     glm::value_ptr(*buffer.data()));
    };

    app.append(std::make_unique<CameraBox>(render.camera(), app.ctx()));
    app.run(callback);
    return EXIT_SUCCESS;
}
