#include <filesystem>
#include <ranges>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/texture_manager.hpp>
#include <OpenGL/image_data.hpp>
#include <OpenGL/image_manager.hpp>
#include <OpenGL/opengl_vertex_input.hpp>
#include <OpenGL/_camera.hpp>

#include <UI/ui.hpp>
#include <UI/io.hpp>

static constexpr int WIDTH = 860;
static constexpr int HEIGHT = 600;

using vertex_t = opengl::vec3pos_vec2tex_t;
using maybe_texdata_t = opengl::TextureManager::maybe_texture_t<
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


struct map_chunk_layer_t final {
    using model_instanced_f = opengl::mat4_instanced;

    opengl::framebuffer_data_t fbuff;
    opengl::InstantRenderData inst_rdata;
    maybe_texdata_t texture = std::nullopt;

public:
    static map_chunk_layer_t create(int w, int h,
                                    maybe_texdata_t tex,
                                    const std::vector<model_instanced_f>& in) {
        map_chunk_layer_t self;
        self.fbuff = opengl::framebuffer_data_t::create(w, h);
        self.inst_rdata = opengl::InstantRenderData::create(
            std::filesystem::path("./pos3_uv2_mod4.vert"),
            std::filesystem::path("./samp2d.frag"),
            create_vertices_xz(),
            create_indices()
        );
        self.inst_rdata.buffers["models"] = model_instanced_f::gen_buffer(
            self.inst_rdata.impl.vao,
            in,
            2
        );
        self.inst_rdata.instance_count = in.size();
        self.texture = tex;
        return self;
    }

    opengl::TextureActivationCommand tex_activation() const {
        if (!texture.has_value()) { throw std::runtime_error("No texture"); }
        const auto& tex = texture.value().get();
        return {
            .tex_unit     = GL_TEXTURE0,
            .sampler_type = tex.target,
            .id           = tex.id,
            .program      = inst_rdata.impl.program,
            .sampler_name = "texture_sampler"
        };
    }

    opengl::fbuff_render_ctx_t fbuff_ctx() const {
        return {
            .fbo             = fbuff.fbo,
            .viewport        = {0, 0, fbuff.texture.w, fbuff.texture.h},
            .screen_viewport = {0, 0, WIDTH, HEIGHT},
            .background      = {0, 0, 0, 255}
        };
    }

    void render(const opengl::_Camera& cam) {
        opengl::fbuff_ctx_guard_t fb_guard(fbuff_ctx());

        auto program = inst_rdata.impl.program;
        opengl::use(program);
        opengl::buffer_bind_guard _({.vao = inst_rdata.impl.vao});
        opengl::activate_texture(tex_activation());
        opengl::set_mat4(program, "projection", cam.projection());
        opengl::set_mat4(program, "view", cam.view());
        opengl::draw_instance_elements({
            .vao           = inst_rdata.impl.vao,
            .count         = GLsizei(inst_rdata.impl.ebo_count),
            .instancecount = inst_rdata.instance_count
        });
        opengl::use(0);
    }
};


struct map_chunk_t final {
    glm::mat4 rotation, scale, translation;
    std::vector<glm::mat4> cell_models;

    opengl::RenderData main_render;
    maybe_texdata_t tex = std::nullopt;
    int cells_w_count, cells_h_count;

    map_chunk_layer_t layer;

public:
    static map_chunk_t create(int cwc, int chc,
                              const glm::vec3& pos,
                              const maybe_texdata_t& tex_data,
                              const maybe_texdata_t& layer_data) {
        map_chunk_t self;
        self.translation = glm::translate(glm::mat4(1.0), pos);
        self.scale = glm::mat4(1.0);
        self.rotation = glm::mat4(1.0);

        self.main_render = opengl::RenderData::create(
            std::filesystem::path("./pos3_uv2.vert"),
            std::filesystem::path("./samp2d.frag"),
            create_vertices_xz(),
            create_indices()
        );
        self.tex           = tex_data;
        self.cells_w_count = cwc;
        self.cells_h_count = chc;
        self.gen_cell_models();

        self.layer = map_chunk_layer_t::create(
            self.cells_w_count,
            self.cells_h_count,
            layer_data,
            opengl::mat4_instanced::convert(self.cell_models)
        );

        return self;
    }

    ~map_chunk_t() {
        main_render.free();
    }

    glm::mat4 model() const {
        return translation * rotation * scale;
    }

    opengl::TextureActivationCommand tex_activation() const {
        return {
            .tex_unit = GL_TEXTURE0,
            .sampler_type = layer.fbuff.texture.target,
            .id = layer.fbuff.texture.id,
            .program = main_render.program,
            .sampler_name = "texture_sampler"
        };
    }

    void render(const opengl::_Camera& cam) {
        auto pr = main_render.program;
        if (!tex.has_value()) {
            std::cerr << "No texture value" << std::endl;
            return;
        }

        layer.render(cam);

        const auto& texture = tex.value().get();
        opengl::use(pr);
        {
        opengl::buffer_bind_guard _({.vao = main_render.vao});
        opengl::activate_texture(tex_activation());
        opengl::set_mat4(pr, "projection", cam.projection());
        opengl::set_mat4(pr, "view", cam.view());
        opengl::set_mat4(pr, "model", model());
        opengl::draw(opengl::DrawElementsCommand {
            .vao   = main_render.vao,
            .count = main_render.ebo_count
        });
        }
        opengl::use(0);
    }

private:
    void gen_cell_models() {
        const size_t area = cells_w_count * cells_h_count;
        const glm::mat4 offset = glm::translate(
            glm::mat4(1.0),
            glm::vec3(-(cells_w_count / 2.0f), 0.0, -(cells_h_count / 2.0f))
        );

        cell_models.clear();
        cell_models = std::vector<glm::mat4>(area);
        for (uint32_t i = 0; i < area; ++i) {
            const uint32_t x = i % cells_w_count;
            const uint32_t z = i / cells_w_count;
            glm::mat4 loc_model = glm::translate(
                glm::mat4(1.0),
                glm::vec3(x + 0.5f, 0.0, z + 0.5f)
            );
            cell_models[i] = offset * loc_model;
            cell_models[i] = cell_models[i] * model();
        }
    }
};


int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }
    auto* win = ui::create_window(WIDTH, HEIGHT, "Map chunks example");
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background(glm::vec4{0.2, 0.6, 1.0, 1.0});
    ui::io::IO::instance().bind(win);

    glm::vec3 cam_pos {0.0, 10.0, 0.0};
    opengl::_Camera camera = opengl::_Camera::create_topdown(
        WIDTH,
        HEIGHT,
        cam_pos
    );

    opengl::ImageManager img_manager({
        {"red", opengl::ImageData::create(1, 1,   {255, 0,   0, 255})},
        {"green", opengl::ImageData::create(1, 1, {0,   255, 0, 255})}
    });
    opengl::TextureManager tex_manager;
    for (const auto& [key, image] : img_manager) {
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

    map_chunk_t map_chunk = map_chunk_t::create(
        2, 2,
        {0.0, 0.0, 0.0},
        tex_manager.get<opengl::TextureData>("red"),
        tex_manager.get<opengl::TextureData>("green")
    );

    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        opengl::Context::instance().draw_background();

        map_chunk.render(camera);

        glfwSwapBuffers(win);
    }

    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}
