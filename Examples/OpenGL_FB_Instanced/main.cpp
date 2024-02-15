#include <filesystem>
#include <ranges>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/texture_manager.hpp>
#include <OpenGL/image_data.hpp>
#include <OpenGL/image_manager.hpp>
#include <OpenGL/opengl_vertex_input.hpp>
#include <OpenGL/camera.hpp>

#include <UI/ui.hpp>
#include <UI/io.hpp>

#include "map.hpp"


static constexpr int WIDTH = 860;
static constexpr int HEIGHT = 600;


int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }
    auto* win = ui::create_window(WIDTH, HEIGHT, "Map chunks example");
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background(glm::vec4{0.1, 0.1, 0.1, 1.0});
    ui::io::IO::instance().bind(win);

    glm::vec3 cam_pos {0.0, 20.0, 0.0};
    opengl::Camera camera = opengl::Camera::create_topdown(
        WIDTH,
        HEIGHT,
        cam_pos
    );

    opengl::ImageManager img_manager({
        {"biomes", opengl::ImageData::create(8, 1, std::vector<glm::u8vec4>{
            biome_to_map[B::ICE],
            biome_to_map[B::TUNDRA],
            biome_to_map[B::FOREST],
            biome_to_map[B::GRASSLANDS],
            biome_to_map[B::MEDITERRANEAN],
            biome_to_map[B::DESERT],
            biome_to_map[B::OCEAN],
            biome_to_map[B::NONE]
        })},
        {"resources", opengl::ImageData::create(1, 4, std::vector<glm::u8vec4>{
            resources_to_map[NR::IRON_ORE],
            resources_to_map[NR::STONE],
            resources_to_map[NR::CLAY],
            resources_to_map[NR::NONE]
        })}
    });
    opengl::TextureManager tex_manager;
    for (const auto& [key, image] : img_manager) {
        opengl::texture_data_array_2d_t tex {
            .tex_data {
                .id         = opengl::gen_texture(GL_TEXTURE_2D_ARRAY),
                .target     = GL_TEXTURE_2D_ARRAY,
                .w          = image.w,
                .h          = image.h,
                .format     = GL_RGBA,
                .type       = GL_UNSIGNED_BYTE,
                .wrap_s     = GL_CLAMP_TO_EDGE,
                .wrap_t     = GL_CLAMP_TO_EDGE,
                .min_filter = GL_LINEAR,
                .mag_filter = GL_LINEAR
            },
            .tile_count_w = (size_t)image.w,
            .tile_count_h = (size_t)image.h
        };
        tex.bind_with_image(image);
        tex_manager.update(key, std::move(tex));
    }

    map_chunk_t map_chunk = map_chunk_t::create(
        4, 4,
        {0.0, 0.0, 0.0},
        std::ref(tex_manager)
    );

    IOHandler io_handler(map_chunk);

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
