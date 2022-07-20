#include <UI/ui.hpp>
#include <OpenGL/opengl.hpp>
#include <OpenGL/item.hpp>


const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;


const std::string_view vertex_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNorm;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const std::string_view orange_fragment_source =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
const std::string_view yellow_fragment_source =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\n\0";


auto create_first_triangle() {
    glm::vec3 normal(0, 0, 1);
    return std::vector<opengl::VertexData> {
        {glm::vec3(-0.9f, -0.5f, 0.0f), normal},
        {glm::vec3(-0.0f, -0.5f, 0.0f), normal},
        {glm::vec3(-0.45f, 0.5f, 0.0f), normal}
    };
}

auto create_second_triangle() {
    glm::vec3 normal(0, 0, 1);
    return std::vector<opengl::VertexData> {
        {glm::vec3(0.0f, -0.5f, 0.0f), normal},
        {glm::vec3(0.9f, -0.5f, 0.0f), normal},
        {glm::vec3(0.45f, 0.5f, 0.0f), normal}
    };
}

auto create_item(auto&& data) {
    opengl::Item item(std::move(data));
    item.initialize();
    return item;
}

int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }
    auto* window = ui::create_window(WIDTH, HEIGHT, "Multiple triangles");

    opengl::Context::instance().initialize();
    opengl::Context::instance().dump();
    opengl::Context::instance().background({0.2f, 0.5f, 0.5f, 1.0f});

    opengl::Program program_orange("orange"), program_yellow("yellow");

    program_orange.attach_shader(GL_VERTEX_SHADER, vertex_shader_source);
    program_orange.attach_shader(GL_FRAGMENT_SHADER, orange_fragment_source);
    program_orange.link_program();

    program_yellow.attach_shader(GL_VERTEX_SHADER, vertex_shader_source);
    program_yellow.attach_shader(GL_FRAGMENT_SHADER, yellow_fragment_source);
    program_yellow.link_program();

    opengl::Item orange_item = create_item(std::move(create_first_triangle())),
                 yellow_item = create_item(std::move(create_second_triangle()));

    while (!glfwWindowShouldClose(window)) {
        opengl::Context::instance().draw_background();

        program_orange.use();
        orange_item.draw();
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        program_yellow.use();
        yellow_item.draw();
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
