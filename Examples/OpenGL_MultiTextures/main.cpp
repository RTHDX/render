#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/texture_manager.hpp>
#include <OpenGL/image_data.hpp>
#include <OpenGL/image_manager.hpp>
#include <OpenGL/opengl_vertex_input.hpp>

#include <UI/ui.hpp>
#include <UI/io.hpp>

static constexpr int WIDTH  = 1920;
static constexpr int HEIGHT = 1000;

using vertex_t = opengl::vec3pos_vec2tex_t;
using maybe_texture_t = opengl::TextureManager::maybe_texture_t<
    opengl::TextureData
>;

std::vector<vertex_t> create_vertices_xz() {
    return {
        {{-1.0, 0.0, -1.0}, {0, 1}},
        {{ 1.0, 0.0, -1.0}, {1, 1}},
        {{ 1.0, 0.0,  1.0}, {1, 0}},
        {{-1.0, 0.0,  1.0}, {0, 0}}
    };
}

std::vector<GLuint> create_indices() {
    return {
        3, 0, 1,
        1, 2, 3
    };
}


struct item_2d {
    glm::mat4 rotation, scale, translation;

    opengl::RenderData render_data;
    maybe_texture_t tex_data = std::nullopt;

public:
    opengl::DrawElementsCommand draw() const {
        return render_data.draw_elements();
    }

    opengl::TextureActivationCommand tex_activation() const {
        if (!tex_data.has_value()) {
            throw std::runtime_error("No texture assigned");
        }
        const auto& t_data = tex_data.value();
        return {
            .tex_unit     = GL_TEXTURE0,
            .sampler_type = t_data.get().target,
            .id = t_data.get().id,
            .program = render_data.program,
            .sampler_name = "texture_sampler",
        };
    }

    glm::mat4 model() const {
        return translation * rotation * scale;
    }
};


std::vector<item_2d> create_items(const std::string& tex_id,
                                  const opengl::TextureManager& tex_manager,
                                  size_t count) {
    std::vector<item_2d> out(count);
    for (auto& item : out) {
        item.render_data = opengl::RenderData::create<vertex_t>(
            std::filesystem::path("./pos3_uv2.vert"),
            std::filesystem::path("./samp2d.frag"),
            create_vertices_xz(),
            create_indices()
        );
        item.tex_data = tex_manager.get<opengl::TextureData>(tex_id);
    }
    return out;
}


int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }
    auto* win = ui::create_window(WIDTH, HEIGHT, "Multiply textures");
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background(glm::vec4{0.2, 0.6, 1.0, 1.0});
    ui::io::IO::instance().bind(win);

    opengl::ImageManager image_manager({
        {"red",   opengl::ImageData::create(1, 1, {255, 0,   0,   255})},
        {"green", opengl::ImageData::create(1, 1, {0,   255, 0,   255})},
        {"blue",  opengl::ImageData::create(1, 1, {0,   0,   255, 255})}
    });
    opengl::TextureManager tex_manager;
    for (const auto& [key, image] : image_manager) {
        opengl::TextureData tex_data {
            .id         = opengl::gen_texture(),
            .target     = GL_TEXTURE_2D,
            .w          = image.w,
            .h          = image.h,
            .format     = GL_RGBA,
            .type       = GL_UNSIGNED_BYTE,
            .wrap_s     = GL_CLAMP_TO_EDGE,
            .wrap_t     = GL_CLAMP_TO_EDGE,
            .min_filter = GL_LINEAR,
            .mag_filter = GL_LINEAR
        };
        tex_data.bind_with_image(image);
        tex_manager.update(key, std::move(tex_data));
    }

    std::vector<item_2d> red_items(10);
    std::vector<item_2d> green_items(10);
    std::vector<item_2d> blue_items(10);

    glfwSetWindowPos(win, 0, 30);
    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        opengl::Context::instance().draw_background();
        glfwSwapBuffers(win);
    }

    glfwDestroyWindow(win);
    glfwTerminate();
    return EXIT_SUCCESS;
}
