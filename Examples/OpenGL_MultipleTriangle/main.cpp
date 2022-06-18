#include <UI/ui.hpp>
#include <OpenGL/opengl.hpp>
#include <OpenGL/item.hpp>


const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;


const std::string_view vertex_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
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


std::vector<float> first_triangle = {
    -0.9f, -0.5f, 0.0f,  // left 
    -0.0f, -0.5f, 0.0f,  // right
    -0.45f, 0.5f, 0.0f,  // top 
};
std::vector<float> second_triangle = {
    0.0f, -0.5f, 0.0f,  // left
    0.9f, -0.5f, 0.0f,  // right
    0.45f, 0.5f, 0.0f   // top 
};

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

    opengl::Item orange_item(first_triangle, opengl::AttribPointer(0, 3, 3)),
                 yellow_item(second_triangle, opengl::AttribPointer(0, 3, 3));

    while (!glfwWindowShouldClose(window)) {
        opengl::Context::instance().draw_background();

        orange_item.draw(program_orange);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        yellow_item.draw(program_yellow);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
