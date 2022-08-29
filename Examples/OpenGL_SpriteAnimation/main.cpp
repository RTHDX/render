#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <UI/ui.hpp>
#include <OpenGL/camera.hpp>
#include <OpenGL/opengl_utils.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/opengl_vertex_input.hpp>
#include <OpenGL/texture.hpp>
#include <OpenGL/opengl_functions.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "3rdParty/stb/stb_image.h"


int WIDTH = 1280;
int HEIGHT = 860;
const std::filesystem::path vertex_path(R"(.\vertex_shader.vert)");
const std::filesystem::path fragment_path(R"(.\fragment_shader.frag)");
const std::filesystem::path image_path(R"(.\hr-level1_running.png)");
glm::vec4 background = { 0.5, 0.8, 0.8, 1.0 };

void frame_preprocess(GLuint id);
void frame_postprocess(auto* w);

std::vector<opengl::vec3pos_vec2tex_t> create_vertices() {
    return {
        {{ 1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}},
        {{ 1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
        {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}}
    };
}

std::vector<uint32_t> create_indices() {
    return {
        0, 1, 3,
        1, 2, 3
    };
}


int main() {
    if (!ui::init_glfw(4, 6)) {
        return EXIT_FAILURE;
    }

    auto* window = ui::create_window(WIDTH, HEIGHT, "Sprite animation");
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background(background);

    auto program = opengl::create_program(vertex_path, fragment_path);
    auto vao = opengl::gen_vertex_array();
    auto ebo = opengl::gen_element_buffer();

    auto vertices = create_vertices();
    auto indices = create_indices();

    auto buffers = decltype(vertices)::value_type::gen_buffers(vao, vertices);
    opengl::bind_vao(vao);
    opengl::bind_ebo(ebo, indices);

    int width, height, depth;
    stbi_set_flip_vertically_on_load(true);
    opengl::byte_t* buffer = stbi_load(R"(.\fire.jpg)",
                                       &width, &height, &depth, STBI_rgb_alpha);
    if (buffer == nullptr) {
        std::cerr << "Could not read texture" << std::endl;
        return EXIT_FAILURE;
    }
    GLuint texture_id;
    const GLenum target = GL_TEXTURE_2D_ARRAY;
    SAFE_CALL(glGenTextures(1, &texture_id));
    SAFE_CALL(glBindTexture(target, texture_id));
    SAFE_CALL(glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0));
    SAFE_CALL(glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, 1));
    SAFE_CALL(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    SAFE_CALL(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    SAFE_CALL(glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    SAFE_CALL(glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    
    const GLsizei tile_w_count = 6, tile_h_count = 6;
    const GLsizei total_tiles = tile_w_count * tile_h_count;
    const GLsizei tile_w = width / tile_w_count,
                  tile_h = height / tile_h_count;
    std::cout << "Texture WxH: " << width << "x" << height;
    std::cout << ", Tile WxH: " << tile_w << "x" << tile_h << std::endl;
    SAFE_CALL(glTexStorage3D(target, 1, GL_RGBA8, tile_w, tile_h,
                             total_tiles));

    SAFE_CALL(glPixelStorei(GL_UNPACK_ROW_LENGTH, width));
    SAFE_CALL(glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, height));
    for (GLsizei i = 0; i < total_tiles; ++i) {
        opengl::function().tex_subimage_3d(
            GL_TEXTURE_2D_ARRAY,
            0,
            0, 0, i,
            tile_w, tile_h, 1,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            buffer
            //buffer
        );
    }

    opengl::function().generate_mipmap(GL_TEXTURE_2D_ARRAY);
    glm::mat4 projection(1.0);
    glm::mat4 view(1.0);
    glm::mat4 model(1.0);
    GLint frame = 0;
    uint64_t count = 0;
    while (!glfwWindowShouldClose(window)) {
        frame_preprocess(vao);
        opengl::Context::instance().draw_background();
        opengl::function().bind_texture(GL_TEXTURE_2D_ARRAY, texture_id);
        //opengl::function().activate_texture(GL_TEXTURE_2D_ARRAY);
        opengl::use(program);
        opengl::set_mat4(program, "projection", projection);
        opengl::set_mat4(program, "view", view);
        opengl::set_mat4(program, "model", model);
        opengl::set_int(program, "total_frames", total_tiles);
        opengl::set_int(program, "current_frame", frame);
        opengl::draw(opengl::DrawElementsCommand{
            .vao=vao,
            .count=indices.size()
        });
        frame = count % total_tiles;
        frame_postprocess(window);
        count = count == std::numeric_limits<uint64_t>::max() ?
            0 : count + 1;

        //using namespace std::chrono_literals;
        //std::this_thread::sleep_for(1s);
    }

    opengl::free_vertex_buffers(buffers);
    opengl::free_vertex_array(vao);
    glfwTerminate();
    stbi_image_free(buffer);
    return 0;
}

void frame_preprocess(GLuint id) {
    opengl::bind_vao(id);
    glfwPollEvents();
}

void frame_postprocess(auto* w) {
    glfwSwapBuffers(w);
    opengl::bind_vao(0);
}
