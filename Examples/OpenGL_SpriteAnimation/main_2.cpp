#include <vector>
#include <iostream>

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


int WIDTH = 1280;
int HEIGHT = 860;
const std::filesystem::path vertex_path(R"(.\vertex_shader.vert)");
const std::filesystem::path fragment_path(R"(.\fragment_shader.frag)");
glm::vec4 background {1.0, 1.0, 1.0, 1.0};

void frame_preprocess(GLuint id);
void frame_postprocess(GLFWwindow* w);

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

    auto* window = ui::create_window(WIDTH, HEIGHT, "Sprite animation 2");
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background(background);

    auto program = opengl::create_program(vertex_path, fragment_path);
    auto vao = opengl::gen_vertex_array();
    auto ebo = opengl::gen_element_buffer();
    auto intf = opengl::get_program_interface(program);
    std::cout << intf << std::endl;

    auto vertices = create_vertices();
    auto indices = create_indices();

    auto buffers = decltype(vertices)::value_type::gen_buffers(vao, vertices);
    opengl::bind_vao(vao);
    opengl::bind_ebo(ebo, indices);

    auto image = opengl::ImageData::read("fire.jpg");
    if (!image.is_valid()) {
        std::cout << "No image read" << std::endl;
        return EXIT_FAILURE;
    }
    opengl::TextureDataArray2D tex_data {
        .tex_data = {
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
        .tile_count_w = 6,
        .tile_count_h = 6
    };

    opengl::set_texture_2d_array_meta(image.data, tex_data);

    glm::mat4 projection(1.0);
    glm::mat4 view(1.0);
    glm::mat4 model(1.0);
    GLint frame = 0;
    uint64_t count = 0;
    while (!glfwWindowShouldClose(window)) {
        frame_preprocess(vao);
        opengl::Context::instance().draw_background();
        opengl::use(program);
        opengl::activate_texture({
            .tex_type     = GL_TEXTURE0,
            .sampler_type = tex_data.tex_data.target,
            .id           = tex_data.tex_data.id,
            .program      = program,
            .sampler_name = "texture_0"
        });
        opengl::set_mat4(program, "projection", projection);
        opengl::set_mat4(program, "view", view);
        opengl::set_mat4(program, "model", model);
        opengl::set_int(program, "total_frames", tex_data.total_tiles());
        opengl::set_int(program, "current_frame", frame);
        opengl::draw(opengl::DrawElementsCommand {
            .vao = vao,
            .count = indices.size()
        });
        frame = count % tex_data.total_tiles();
        frame_postprocess(window);
        count = count == std::numeric_limits<uint64_t>::max() ?
            0 : count + 1;
    }

    opengl::free_vertex_buffers(buffers);
    opengl::free_vertex_array(vao);
    glfwTerminate();
    return 0;
}

void frame_preprocess(GLuint id) {
    opengl::bind_vao(id);
    glfwPollEvents();
}

void frame_postprocess(GLFWwindow* w) {
    glfwSwapBuffers(w);
    opengl::bind_vao(0);
}
