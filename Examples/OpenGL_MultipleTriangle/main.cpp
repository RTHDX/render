#include <ui.hpp>
#include <opengl.hpp>


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


float first_triangle[] = {
    -0.9f, -0.5f, 0.0f,  // left 
    -0.0f, -0.5f, 0.0f,  // right
    -0.45f, 0.5f, 0.0f,  // top 
};
float second_triangle[] = {
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

    GLuint program_orange = opengl::create_program(
        vertex_shader_source,
        orange_fragment_source
    );
    GLuint program_yellow = opengl::create_program(
        vertex_shader_source,
        yellow_fragment_source
    );

    uint32_t vao_1, vao_2;
    glGenVertexArrays(1, &vao_1);
    glGenVertexArrays(1, &vao_2);
    uint32_t vbo_1, vbo_2;
    glGenBuffers(1, &vbo_1);
    glGenBuffers(1, &vbo_2);

    glBindVertexArray(vao_1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_1);
    glBufferData(GL_ARRAY_BUFFER, sizeof (first_triangle), first_triangle,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(vao_2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_2);
    glBufferData(GL_ARRAY_BUFFER, sizeof (second_triangle), second_triangle,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (float), nullptr);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        opengl::Context::instance().draw_background();

        glUseProgram(program_orange);
        glBindVertexArray(vao_1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(program_yellow);
        glBindVertexArray(vao_2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
