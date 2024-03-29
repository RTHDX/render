#include <filesystem>
#include <ranges>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/texture_manager.hpp>
#include <OpenGL/image_data.hpp>
#include <OpenGL/image_manager.hpp>
#include <OpenGL/opengl_vertex_input.hpp>
#include <OpenGL/camera.hpp>

#include <UI/ui.hpp>
#include <UI/io.hpp>

static constexpr int WIDTH  = 860;
static constexpr int HEIGHT = 600;

using vertex_t = opengl::vec3pos_vec2tex_t;
using maybe_texture_t = opengl::TextureManager::maybe_texture_t<
    opengl::texture_data_t
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

    opengl::render_data_t render_data;
    maybe_texture_t tex_data = std::nullopt;

public:
    opengl::draw_elements_command_t draw() const {
        return render_data.draw_elements();
    }

    opengl::texture_activation_command_t tex_activation() const {
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
        item.render_data = opengl::render_data_t::create<vertex_t>(
            std::filesystem::path("./pos3_uv2.vert"),
            std::filesystem::path("./samp2d.frag"),
            create_vertices_xz(),
            create_indices()
        );
        item.tex_data = tex_manager.get<opengl::texture_data_t>(tex_id);
        item.rotation = glm::mat4(1.0);
        item.scale = glm::scale(glm::mat4(1.0), {0.3, 0.0, 0.3});
    }
    return out;
}

void update_pos(std::vector<item_2d>& items, glm::vec3 pos, glm::vec3 step) {
    for (auto& item : items) {
        item.translation = glm::translate(glm::mat4(1.0), pos);
        pos = pos + step;
    }
}

void render(const std::vector<item_2d>& items,
            const opengl::Camera& camera) {
    for (const auto& item : items) {
        auto program = item.render_data.program;
        opengl::use(program);
        opengl::bind_vao(item.render_data.vao);

        opengl::activate_texture(item.tex_activation());
        opengl::set_mat4(program, "projection", camera.projection());
        opengl::set_mat4(program, "view", camera.view());
        opengl::set_mat4(program, "model", item.model());
        opengl::draw(item.draw());

        opengl::bind_vao(0);
        opengl::use(0);
    }
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
        opengl::texture_data_t tex_data {
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

    std::vector<item_2d> red_items = create_items("red", tex_manager, 10);
    update_pos(red_items, {-5.0, 0.0, -5.0}, {0.0, 0.0, 1.0});

    std::vector<item_2d> blue_items = create_items("blue", tex_manager, 10);
    update_pos(blue_items, {0.0, 0.0, -5.0}, {0.0, 0.0, 1.0});

    std::vector<item_2d> green_items = create_items("green", tex_manager, 10);
    update_pos(green_items, {5.0, 0.0, -5.0}, {0.0, 0.0, 1.0});

    std::vector<std::reference_wrapper<const std::vector<item_2d>>>
    all_items_refs = {red_items, blue_items, green_items};

    glm::vec3 cam_pos {0.0, 30.0, 0.0};
    opengl::Camera camera = opengl::Camera::create_topdown(
        WIDTH,
        HEIGHT,
        cam_pos.y / 1000,
        cam_pos
    );

    glfwSetWindowPos(win, 0, 30);
    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        opengl::Context::instance().draw_background();

        render(red_items, camera);
        render(blue_items, camera);
        render(green_items, camera);

        glfwSwapBuffers(win);
    }

    glfwDestroyWindow(win);
    glfwTerminate();
    return EXIT_SUCCESS;
}
