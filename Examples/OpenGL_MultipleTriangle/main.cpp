#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <UI/ui.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/opengl_vertex_input.hpp>


const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;


const std::string vertex_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNorm;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const std::string orange_fragment_source =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
const std::string yellow_fragment_source =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\n\0";

using VertexData = opengl::vec3pos_vec3norm_t;
auto create_first_triangle() {
    return std::vector<VertexData> {
        {glm::vec3(-0.9f, -0.5f, 0.0f), glm::vec3{0, 0, 1}},
        {glm::vec3(-0.0f, -0.5f, 0.0f), glm::vec3{0, 0, 1}},
        {glm::vec3(-0.45f, 0.5f, 0.0f), glm::vec3{0, 0, 1}}
    };
}

auto create_second_triangle() {
    return std::vector<VertexData> {
        {glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(0, 0, 1)},
        {glm::vec3(0.9f, -0.5f, 0.0f), glm::vec3(0, 0, 1)},
        {glm::vec3(0.45f, 0.5f, 0.0f), glm::vec3(0, 0, 1)}
    };
}

int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }
    auto* window = ui::create_window(WIDTH, HEIGHT, "Multiple triangles");

    opengl::Context::instance().initialize();
    opengl::Context::instance().dump();
    opengl::Context::instance().background(glm::vec4{0.2f, 0.5f, 0.5f, 1.0f});

    std::vector<std::vector<VertexData>> objects {
        create_first_triangle(),
        create_second_triangle()
    };
    std::vector<GLuint> programs {
        opengl::create_program(vertex_shader_source,
                               orange_fragment_source),
        opengl::create_program(vertex_shader_source,
                               yellow_fragment_source)
    };
    std::vector<GLuint> vao = opengl::gen_vertex_array(2),
                        pos_vbo = opengl::gen_vertex_buffers(2),
                        norm_vbo = opengl::gen_vertex_buffers(2);
    for (size_t i = 0; i < vao.size(); ++i) {
        opengl::bind_vao(vao[i]);

        opengl::bind_vbo<VertexData>(pos_vbo[i], objects[i]);
        opengl::bind_vbo<VertexData>(norm_vbo[i], objects[i]);
        opengl::do_vertex_attrib_cmds(std::move(VertexData::commands()));

        opengl::bind_vao(0);
    }

    while (!glfwWindowShouldClose(window)) {
        opengl::Context::instance().draw_background();

        for (size_t i = 0; i < vao.size(); ++i) {
            opengl::use(programs[i]);
            opengl::draw(opengl::draw_array_command_t{.vao=vao[i], .count=objects[i].size()});
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    opengl::free_vertex_array(vao);
    opengl::free_vertex_buffers(pos_vbo);
    opengl::free_vertex_buffers(norm_vbo);
    for (auto program : programs) {
        opengl::free_program(program);
    }

    glfwTerminate();
    return 0;
}
