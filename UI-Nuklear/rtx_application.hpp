#pragma once

#include <string>
#include <string_view>

#include <RTX/scene.hpp>
#include <RTX/render.hpp>

#include "nuklear_widgets.hpp"

struct GLFWwindow;
struct nk_context;

namespace ui::nuklear {
static constexpr char FLOAT_TEMPL[] = "{:.4f}";
static constexpr char VEC3_TEMPLATE[] = "{:.4f} | {:.4f} | {:.4f}";


class CameraMenu final : public ui::nuklear::Widget {
public:
    CameraMenu(const rtx::Camera& camera);

    void show(struct nk_context*) override;

private:
    void render_position(struct nk_context*);
    void render_target(struct nk_context*);
    void render_view(struct nk_context*);
    void render_aspect_ratio(struct nk_context*);

private:
    const rtx::Camera& _camera;
};


template<rtx::Traceable T> class SceneInfo : public Widget {
public:
    SceneInfo(const rtx::Scene<T>& scene)
        : Widget("Scene info")
        , _scene(scene)
    {}

    void show(struct nk_context*) override;

private:
    void render_mesh_count(struct nk_context*);
    void render_meshes(struct nk_context*);

    void render_object(struct nk_context* ctx, const rtx::Sphere& sphere) {}
    void render_object(struct nk_context* ctx, const rtx::Triangle& triangle);
    void render_object(struct nk_context* ctx, const rtx::Mesh& mesh);

private:
    const rtx::Scene<T>& _scene;
};

template <rtx::Traceable T>
void SceneInfo<T>::show(struct nk_context* ctx) {
    if (nk_tree_push(ctx, NK_TREE_TAB, "Scene", NK_MINIMIZED)) {
        render_mesh_count(ctx);
        render_meshes(ctx);
        nk_tree_pop(ctx);
    }
}

template <rtx::Traceable T>
void SceneInfo<T>::render_mesh_count(struct nk_context* ctx) {
    std::string content(std::format("Meshes in scene: {}",
        _scene.objects.size()));
    nk_label(ctx, content.data(), NK_TEXT_LEFT);
}

template <rtx::Traceable T>
void SceneInfo<T>::render_meshes(struct nk_context* ctx) {
    for (const auto& object : _scene.objects) {
        render_object(ctx, object);
    }
}

template <rtx::Traceable T>
void SceneInfo<T>::render_object(struct nk_context* ctx,
    const rtx::Triangle& tr) {
    nk_label(ctx,
        std::format(VEC3_TEMPLATE, tr.a.x, tr.a.y, tr.a.z).data(),
        NK_TEXT_LEFT);
    nk_label(ctx,
        std::format(VEC3_TEMPLATE, tr.b.x, tr.b.y, tr.b.z).data(),
        NK_TEXT_LEFT);
    nk_label(ctx,
        std::format(VEC3_TEMPLATE, tr.c.x, tr.c.y, tr.c.z).data(),
        NK_TEXT_LEFT);
}

template <rtx::Traceable T>
void SceneInfo<T>::render_object(struct nk_context* ctx, const rtx::Mesh& mesh) {
    if (nk_tree_push(ctx, NK_TREE_TAB, mesh.name.c_str(), NK_MINIMIZED)) {
        for (const auto& face : mesh.faces) {
            render_object(ctx, face);
        }
        nk_tree_pop(ctx);
    }
}


class RTX_App {
public:
    RTX_App() = delete;
    RTX_App(const std::string_view title, size_t width, size_t height);

    ~RTX_App() {
        nk_glfw3_shutdown();
        glfwTerminate();
    }

    bool initialize();

    template <typename T> void run(T& render) {
        initialize_ui(render);

        while (!glfwWindowShouldClose(_window)) {
            pre_render();

            for (auto& widget : _widgets) {
                widget->show(_ctx);
            }

            BENCHMARK(render.render());
            const auto& frame = render.buffer();
            glDrawPixels(_width, _height, GL_RGB, GL_FLOAT,
                         glm::value_ptr(*frame.data()));

            post_render();
        }
    }

private:
    template<typename R> void initialize_ui(const R& render) {
        int width, height;
        glfwGetWindowSize(_window, &width, &height);
        uWidget main(std::make_unique<Window>(
            "Info panel", nk_rect(0, 0, 300, height),
            NK_WINDOW_TITLE | NK_WINDOW_MINIMIZABLE
            ));
        main->append(std::move(
            std::make_unique<CameraMenu>(render.camera())
        ));
        main->append(std::move(
            std::make_unique<SceneInfo<rtx::Mesh>>(render.scene())
        ));
        _widgets.push_back(std::move(main));
    }

    void pre_render();
    void post_render();

private:
    std::string _title;
    size_t _width, _height;
    GLFWwindow* _window = nullptr;
    nk_context* _ctx = nullptr;

    std::vector<uWidget> _widgets;
};

}
