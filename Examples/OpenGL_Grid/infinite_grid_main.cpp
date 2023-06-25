#include <iostream>
#include <filesystem>

#include <glm/gtx/transform.hpp>

#include "infinite_grid_decls.hpp"


namespace fs = std::filesystem;
using vertex_t = opengl::vec3pos;


int WIDTH = 800;
int HEIGHT = 600;


std::vector<vertex_t> create_vertices_xz() {
    return {
        {{-1.0, 0.0, -1.0}},
        {{ 1.0, 0.0, -1.0}},
        {{ 1.0, 0.0,  1.0}},
        {{-1.0, 0.0,  1.0}}
    };
}

std::vector<GLuint> create_indices() {
    return {
        3, 0, 1,
        1, 2, 3
    };
}


int main() {
    if (!ui::init_glfw(4, 6)) { return 1; }

    auto* win = ui::create_window(WIDTH, HEIGHT, "Infinite grid");
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background(glm::vec4{0.2, 0.6, 1.0, 1.0});
    ui::io::IO::instance().bind(win);
    LocalListener listener;

    auto program = opengl::create_program(
        fs::path("./gpt_pos3.vert"),
        fs::path("./gpt_grid.frag")
    );
    std::cout << opengl::get_program_interface(program) << std::endl;
    auto vertex_data = create_vertices_xz();
    auto element_data = create_indices();
    auto vao = opengl::gen_vertex_array();
    auto ebo = opengl::gen_element_buffer();
    auto vbos = vertex_t::gen_buffers(vao, vertex_data, ebo, element_data);

    opengl::OrthoCamera camera(
        {0.0, 10.0, 0.0},
        float(WIDTH), float(HEIGHT),
        0.1
    );
    listener.camera = &camera;
    listener.win = win;

    glm::mat4 model = glm::scale(glm::mat4(1.0), glm::vec3(100, 1.0, 100));
    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        opengl::Context::instance().draw_background();

        opengl::use(program);
        opengl::set_mat4(program, "projection",
                         camera.projection());
        opengl::set_mat4(program, "view", camera.view());
        opengl::set_mat4(program, "model", model);
        opengl::set_vec3(program, "grid_color", {1.0, 1.0, 0.801});
        opengl::set_float(program, "grid_size", 10.0f);
        opengl::set_float(program, "line_width", 0.3f);
        opengl::draw(opengl::DrawElementsCommand{
            .vao = vao,
            .count = element_data.size()
        });
        opengl::use(0);

        glfwSwapBuffers(win);
    }

    opengl::free_vertex_array(vao);
    opengl::free_vertex_buffers(vbos);
    opengl::free_program(program);
    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}
