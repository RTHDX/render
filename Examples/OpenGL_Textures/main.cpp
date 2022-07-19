#include <filesystem>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <UI/ui.hpp>
#include <OpenGL/opengl.hpp>
#include <OpenGL/camera.hpp>
#include <OpenGL/opengl_utils.hpp>

int WIDTH = 1280;
int HEIGHT = 860;


std::vector<glm::vec3> gen_frame(size_t width, size_t height) {
    std::vector<glm::vec3> out(width * height);
    for (size_t h_pos = 0; h_pos < height; ++h_pos) {
        for (size_t w_pos = 0; w_pos < width; ++w_pos) {
            const size_t i = h_pos * width + w_pos;
            out[i] = {1.0, 1.0, 1.0};
        }
    }
    return out;
}

struct TexVertexData {
    glm::vec3 pos;
    glm::vec2 tex_pos;
};


auto create_camera() {
    return opengl::Camera {(float)WIDTH, (float)HEIGHT,
        glm::radians(45.0),
        {10, 10, 10}
    };
}


int main() {
    if (!ui::init_glfw(4, 6)) {
        std::cerr << "Could not load glfw" << std::endl;
        return EXIT_FAILURE;
    }

    auto* window = ui::create_window(WIDTH, HEIGHT, "Textures");
    opengl::Context::instance().initialize(true);

    auto program = opengl::create_program(
        std::filesystem::path("./vertex_shader.vert"),
        std::filesystem::path("./fragment_shader.frag")
    );

    std::vector<TexVertexData> vertices{
        {.pos={-1.0,  1.0, 0.0}, .tex_pos={0.0, 1.0}},
        {.pos={ 1.0,  1.0, 0.0}, .tex_pos={1.0, 1.0}},
        {.pos={ 1.0, -1.0, 0.0}, .tex_pos={1.0, 0.0}},
        {.pos={-1.0, -1.0, 0.0}, .tex_pos={0.0, 0.0}}
    };
    GLuint vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof (TexVertexData),
                 vertices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)offsetof(TexVertexData, pos));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)offsetof(TexVertexData, tex_pos));
    glEnableVertexAttribArray(1);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto frame = gen_frame(WIDTH, HEIGHT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT,
                 glm::value_ptr(*frame.data()));
    glGenerateMipmap(GL_TEXTURE_2D);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, tex);

        glUseProgram(program);
        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return EXIT_SUCCESS;
}
