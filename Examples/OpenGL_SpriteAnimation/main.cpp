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

using VertexData = opengl::vec3pos;

std::vector<VertexData> create_square_vec3() {
    return {
        {glm::vec3{-1.0,  1.0, 0.0}},
        {glm::vec3{ 1.0,  1.0, 0.0}},
        {glm::vec3{ 1.0, -1.0, 0.0}},

        {glm::vec3{-1.0,  1.0, 0.0}},
        {glm::vec3{-1.0, -1.0, 0.0}},
        {glm::vec3{1.0,  -1.0, 0.0}}
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

    GLuint vao = opengl::gen_vertex_array(),
           pos_vbo = opengl::gen_vertex_buffers();
           //norm_vbo = opengl::gen_vertex_buffers(),
           //tex_vbo = opengl::gen_vertex_buffers();

    auto vertices = create_square();

    opengl::bind_vao(vao);
    opengl::bind_vbo<VertexData>(pos_vbo, vertices);
    opengl::do_vertex_attrib_cmds(std::move(VertexData::commands()));

    glm::mat4 projection(1.0);
    glm::mat4 view(1.0);
    glm::mat4 model(1.0);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        opengl::Context::instance().draw_background();
        opengl::use(program);
        opengl::set_mat4(program, "projection", projection);
        opengl::set_mat4(program, "view", view);
        opengl::set_mat4(program, "model", model);
        opengl::draw({.vao=vao, .count=vertices.size()});

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
