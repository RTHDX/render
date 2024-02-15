#include <array>
#include <vector>
#include <filesystem>
#include <iostream>
#include <chrono>
#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <UI/ui.hpp>
#include <OpenGL/texture.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/opengl_vertex_input.hpp>
#include <OpenGL/camera.hpp>

#include <UI/io.hpp>

namespace fs = std::filesystem;
using namespace std::chrono_literals;

using vertex_input_t = opengl::vec3pos_vec2tex_t;
using instanced_input_t = opengl::mat4_f_instanced;

static constexpr int WIDTH = 800;
static constexpr int HEIGHT = 600;
static constexpr size_t SQUARES = 4;
static constexpr float MOVE = 0.51;
static constexpr float CELL_SIZE = 1.0;
static constexpr glm::mat4 ONE(1.0);

static glm::mat4 SCALE_MAT = glm::scale(ONE, glm::vec3(0.4));

const std::vector<vertex_input_t>& vertex_input() {
    static std::vector<vertex_input_t> out{
        {{ CELL_SIZE,  CELL_SIZE, 0.0f}, {1.0f, 1.0f}},
        {{ CELL_SIZE, -CELL_SIZE, 0.0f}, {1.0f, 0.0f}},
        {{-CELL_SIZE, -CELL_SIZE, 0.0f}, {0.0f, 0.0f}},
        {{-CELL_SIZE,  CELL_SIZE, 0.0f}, {0.0f, 1.0f}}
    };
    return out;
}

const std::vector<GLuint> vertex_indices() {
    static std::vector<GLuint> out{
        0, 1, 3,
        1, 2, 3
    };
    return out;
}

float get_random_ds() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.01, 0.1);
    return dis(gen);
}


const glm::u8vec4 RED        {255, 0,   0,   255};
const glm::u8vec4 DARK_RED   {125, 0,   0,   255};

const glm::u8vec4 BLUE       {0,   0,   255, 255};
const glm::u8vec4 DARK_BLUE  {0,   0,   125, 255};

const glm::u8vec4 GREEN      {0,   255, 0,   255};
const glm::u8vec4 DARK_GREEN {0,   125, 0,   255};

const glm::u8vec4 WHITE      {255, 255, 255, 255};
const glm::u8vec4 GRAY       {125, 125, 125, 255};


std::vector<glm::mat4> models {
    glm::translate(ONE, glm::vec3(-MOVE,  MOVE, 0.0f)) * SCALE_MAT,
    glm::translate(ONE, glm::vec3( MOVE,  MOVE, 0.0f)) * SCALE_MAT,
    glm::translate(ONE, glm::vec3(-MOVE, -MOVE, 0.0f)) * SCALE_MAT,
    glm::translate(ONE, glm::vec3( MOVE, -MOVE, 0.0f)) * SCALE_MAT,
};

std::vector<float> frames {
    0.0,
    1.0,
    2.0,
    3.0
};


int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }
    auto* win = ui::create_window(WIDTH, HEIGHT,
        "Instance Drawing with samp2dArray Animated"
    );
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background(glm::vec4{0.2, 0.6, 1.0, 1.0});
    ui::io::IO::instance().bind(win);

    auto program = opengl::create_program(
        fs::path("./vec3_vec2_mat4_f.vert"),
        fs::path("./samp_2D_array.frag")
    );
    auto program_intf = opengl::get_program_interface(program);
    std::cout << program_intf << std::endl;

    const auto& vertices = vertex_input();
    const auto& elements = vertex_indices();

    GLuint vao = opengl::gen_vertex_array();
    GLuint ebo = opengl::gen_element_buffer();
    auto buffers  = vertex_input_t::gen_buffers(vao, vertices, ebo, elements);
    auto models_converted = opengl::mat4_instanced::convert(models);
    GLuint mat_i_buffer = opengl::mat4_instanced::gen_buffer(
        vao,
        models_converted,
        2,
        GL_DYNAMIC_DRAW
    );
    auto frames_converted = opengl::float_instanced::convert(frames);
    GLuint f_i_buffer = opengl::float_instanced::gen_buffer(
        vao,
        frames_converted,
        6,
        GL_DYNAMIC_DRAW
    );

    glm::mat4 proj(1.0), view(1.0);

    int tiles_count_w = 2;
    int tiles_count_h = 4;
    auto image = opengl::ImageData::create(tiles_count_w, tiles_count_h,
        std::vector<glm::u8vec4> {
            RED,   DARK_RED,
            BLUE,  DARK_BLUE,
            GREEN, DARK_GREEN,
            WHITE, GRAY,
        }
    );
    image.dump();

    opengl::texture_data_array_2d_t tex2d_data{
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
        .tile_count_w = size_t(tiles_count_w),
        .tile_count_h = size_t(tiles_count_h)
    };
    opengl::set_texture_2d_array_meta(image.data, tex2d_data);

    opengl::texture_activation_command_t tex_activation{
        .tex_unit     = GL_TEXTURE0,
        .sampler_type = tex2d_data.tex_data.target,
        .id           = tex2d_data.tex_data.id,
        .program      = program,
        .sampler_name = "textureArray"
    };

    bool is_odd = false;
    auto t0 = std::chrono::system_clock::now();
    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        opengl::Context::instance().draw_background();

        opengl::use(program);
        opengl::activate_texture(tex_activation);
        opengl::set_mat4(program, "projection", proj);
        opengl::set_mat4(program, "view", view);
        opengl::draw_instance_elements({
            .vao           = vao,
            .count         = GLsizei(elements.size()),
            .instancecount = GLsizei(frames.size())
        });
        opengl::use(0);

        frames_converted[0] = is_odd ? 0.0 : 1.0;
        frames_converted[1] = is_odd ? 2.0 : 3.0;
        frames_converted[2] = is_odd ? 4.0 : 5.0;
        frames_converted[3] = is_odd ? 6.0 : 7.0;

        auto t1 = std::chrono::system_clock::now();
        if ((t1 - t0) > 1s) {
            t0 = t1;
            is_odd = !is_odd;
            opengl::float_instanced::update(f_i_buffer, frames_converted);
        }

        glfwSwapBuffers(win);
    }

    opengl::free_vertex_buffers(buffers);
    opengl::free_vertex_buffer(mat_i_buffer);
    opengl::free_vertex_buffer(f_i_buffer);
    opengl::free_element_buffer(ebo);
    opengl::free_vertex_array(vao);

    glfwDestroyWindow(win);
    glfwTerminate();
    return EXIT_SUCCESS;
}
