#include <array>
#include <vector>
#include <filesystem>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <UI/ui.hpp>
#include <OpenGL/texture.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/opengl_vertex_input.hpp>
#include <OpenGL/camera.hpp>

#include <UI/io.hpp>

namespace fs = std::filesystem;

static constexpr int WIDTH = 800;
static constexpr int HEIGHT = 600;

float vertices_square[] = {
    0.5f,  0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
   -0.5f, -0.5f, 0.0f,

   -0.5f, -0.5f, 0.0f,
   -0.5f,  0.5f, 0.0f,
    0.5f,  0.5f, 0.0f,
};

float vertices_triangle[] = {
    //-1.0f,  1.0f, 0.0f,
    // 1.0f,  1.0f, 0.0f,
    // 1.0f, -1.0f, 0.0f

    0.5f,  0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
   -0.5f, -0.5f, 0.0f,
};

float vertices_screen_square[] = {
    // координаты      // текстурные координаты
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
};

glm::vec3 RED  {1.0, 0.0, 0.0};
glm::vec3 BLUE {0.0, 0.0, 1.0};


int main() {
    if (!ui::init_glfw(4, 6)) { return EXIT_FAILURE; }

    auto* win = ui::create_window(WIDTH, HEIGHT, "Framebuffer rendering");
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background(glm::vec4{0.0, 0.8, 0.0, 1.0});
    ui::io::IO::instance().bind(win);

    GLuint program_left = opengl::create_program(
        fs::path("./vec3pos.vert"),
        fs::path("./fixed_color.frag")
    );
    GLuint program_right = opengl::create_program(
        fs::path("./vec3pos.vert"),
        fs::path("./fixed_color.frag")
    );

    std::cout << opengl::get_program_interface(program_left) << std::endl;
    std::cout << opengl::get_program_interface(program_right) << std::endl;

    GLuint vao_left, vao_right,
           vbo_left, vbo_right;

    glGenVertexArrays(1, &vao_left);
    glGenVertexArrays(1, &vao_right);

    glGenBuffers(1, &vbo_left);
    glGenBuffers(1, &vbo_right);

    {
        glBindVertexArray(vao_left);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_left);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_square), vertices_square,
                     GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void*)0);
        glBindVertexArray(0);
    }

    {
        glBindVertexArray(vao_right);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_right);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangle),
                     vertices_triangle, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void*)0);
        glBindVertexArray(0);
    }

    GLuint texture_left, texture_right;
    glGenTextures(1, &texture_left);
    glGenTextures(1, &texture_right);
    {
        glBindTexture(GL_TEXTURE_2D, texture_left);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH / 2, HEIGHT, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    {
        glBindTexture(GL_TEXTURE_2D, texture_right);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH / 2, HEIGHT, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLuint fbo_left, fbo_right;
    glGenFramebuffers(1, &fbo_left);
    glGenFramebuffers(1, &fbo_right);
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_left);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, texture_left, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo_right);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, texture_right, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLuint screen_vao, screen_vbo;
    glGenVertexArrays(1, &screen_vao);
    glGenBuffers(1, &screen_vbo);
    {
        // Заполнение VBO данными о квадратах и связывание его с VAO
        glBindVertexArray(screen_vao);
        glBindBuffer(GL_ARRAY_BUFFER, screen_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_screen_square),
                     vertices_screen_square, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void*)(3 * sizeof(float)));
        glBindVertexArray(0);
    }

    GLuint program = opengl::create_program(
        fs::path("./vec3pos_vec2uv.vert"),
        fs::path("./vec2uv.frag")
    );

    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        opengl::Context::instance().draw_background();

        {
            // Рендеринг в левый фреймбуфер
            glBindFramebuffer(GL_FRAMEBUFFER, fbo_left);
            glClearColor(1.0f, 1.0f, 0.0f, 0.5f);
            glClear(GL_COLOR_BUFFER_BIT);
            glUseProgram(program_left);
            glBindVertexArray(vao_left);
            glViewport(0, 0, WIDTH / 2, HEIGHT); // Задаем размеры видового окна для левой половины экрана
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        {
            // Рендеринг в правый фреймбуфер
            glBindFramebuffer(GL_FRAMEBUFFER, fbo_right);
            glClearColor(1.0f, 1.0f, 0.0f, 0.5f);
            glClear(GL_COLOR_BUFFER_BIT);
            glUseProgram(program_right);
            glBindVertexArray(vao_right);
            glViewport(WIDTH / 2, 0, WIDTH / 2, HEIGHT); // Задаем размеры видового окна для правой половины экрана
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        // Рендеринг текстуры левого фреймбуфера на экране
        glUseProgram(program);
        {
            glBindVertexArray(screen_vao);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_left);
            GLint tex_location = glGetUniformLocation(program, "tex");
            glViewport(0, 0, WIDTH / 2, HEIGHT); // Задаем размеры видового окна для левой половины экрана
            glUniform1i(tex_location, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }

        glUseProgram(program);
        {
            // Рендеринг текстуры правого фреймбуфера на экране
            glBindVertexArray(screen_vao);
            glActiveTexture(GL_TEXTURE1);  // Use texture unit 1
            glBindTexture(GL_TEXTURE_2D, texture_right);
            GLint tex_location = glGetUniformLocation(program, "tex");
            glViewport(WIDTH / 2, 0, WIDTH / 2, HEIGHT);
            glUniform1i(tex_location, 1);  // Tell the shader to use texture unit 1
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }

        glViewport(0, 0, WIDTH, HEIGHT);
        glfwSwapBuffers(win);
    }

    glDeleteTextures(1, &texture_left);
    glDeleteTextures(1, &texture_right);

    glDeleteFramebuffers(1, &fbo_left);
    glDeleteFramebuffers(1, &fbo_right);

    glDeleteBuffers(1, &vbo_right);
    glDeleteBuffers(1, &vbo_left);

    glDeleteVertexArrays(1, &vao_right);
    glDeleteVertexArrays(1, &vao_left);

    glDeleteProgram(program_left);
    glDeleteProgram(program_right);
    glDeleteProgram(program);

    glfwDestroyWindow(win);
    glfwTerminate();
}