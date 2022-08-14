#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <UI/ui.hpp>
#include <OpenGL/camera.hpp>
#include <OpenGL/opengl_utils.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/opengl_vertex_input.hpp>
#include <OpenGL/texture.hpp>


int WIDTH = 1280;
int HEIGHT = 860;
const std::filesystem::path vertex_path(R"(.\vertex_shader.vert)");
const std::filesystem::path fragment_path(R"(.\fragment_shader.frag)");
const std::filesystem::path image_path(R"(.\hr-level1_running.png)");
glm::vec4 background = { 0.5, 0.8, 0.8, 1.0 };

void frame_preprocess(GLuint id);
void frame_postprocess(auto* w);

std::vector<opengl::vec3pos> create_square_vec3() {
    return {
        {glm::vec3{-1.0,  1.0, 0.0}},
        {glm::vec3{ 1.0,  1.0, 0.0}},
        {glm::vec3{ 1.0, -1.0, 0.0}},

        {glm::vec3{-1.0,  1.0, 0.0}},
        {glm::vec3{-1.0, -1.0, 0.0}},
        {glm::vec3{1.0,  -1.0, 0.0}}
    };
}

std::vector<opengl::vec3pos_vec2tex_t> create_square_v3p_v2t() {
    return {
        {glm::vec3{-1.0,  1.0, 0.0}, glm::vec2{0, 1}},
        {glm::vec3{ 1.0,  1.0, 0.0}, glm::vec2{1, 1}},
        {glm::vec3{ 1.0, -1.0, 0.0}, glm::vec2{1, 0}},

        {glm::vec3{-1.0,  1.0, 0.0}, glm::vec2{0, 1}},
        {glm::vec3{-1.0, -1.0, 0.0}, glm::vec2{0, 0}},
        {glm::vec3{1.0,  -1.0, 0.0}, glm::vec2{1, 0}}
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
    opengl::Texture texture("hr-level1_running.png");
    if (texture.read()) {
        texture.id = opengl::gen_texture();
        opengl::bind_texture({WIDTH, HEIGHT}, texture.buffer);
    }

    GLuint vao = opengl::gen_vertex_array();
    auto vertices = create_square_vec3();
    auto buffers = decltype(vertices)::value_type::gen_buffers(vao, vertices);

    glm::mat4 projection(1.0);
    glm::mat4 view(1.0);
    glm::mat4 model(1.0);
    while (!glfwWindowShouldClose(window)) {
        frame_preprocess(vao);

        opengl::Context::instance().draw_background();
        opengl::use(program);
        opengl::activate_texture(texture.id);
        opengl::set_mat4(program, "projection", projection);
        opengl::set_mat4(program, "view", view);
        opengl::set_mat4(program, "model", model);
        opengl::draw(opengl::DrawArrayCommand{.vao=vao, .count=vertices.size()});

        frame_postprocess(window);
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

void frame_postprocess(auto* w) {
    glfwSwapBuffers(w);
    opengl::bind_vao(0);
}
