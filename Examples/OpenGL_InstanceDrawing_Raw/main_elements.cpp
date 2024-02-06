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
static constexpr size_t SQUARES = 4;
static constexpr float MOVE = 0.51;
static glm::mat4 SCALE_MAT = glm::scale(glm::mat4(1.0), glm::vec3(0.5));


std::array<glm::mat4, SQUARES> model_matrices = {
    glm::translate(glm::mat4(1.0f), glm::vec3(-MOVE,  MOVE, 0.0f)) * SCALE_MAT,
    glm::translate(glm::mat4(1.0f), glm::vec3(MOVE,  MOVE, 0.0f)) * SCALE_MAT,
    glm::translate(glm::mat4(1.0f), glm::vec3(-MOVE, -MOVE, 0.0f)) * SCALE_MAT,
    glm::translate(glm::mat4(1.0f), glm::vec3(MOVE, -MOVE, 0.0f)) * SCALE_MAT
};


float vertices[] = {
    // координаты      // текстурные координаты
    0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // Вершина 0
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Вершина 1
   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Вершина 2
   -0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // Вершина 3
};


GLuint indices[] = {
    0, 1, 3,
    1, 2, 3
};


int main() {
    if (!ui::init_glfw(4, 6)) { return 1; }

    auto* win = ui::create_window(WIDTH, HEIGHT, "Instance Element Drawing Raw");
    opengl::Context::instance().initialize(true);
    opengl::Context::instance().background(glm::vec4{0.2, 0.6, 1.0, 1.0});
    ui::io::IO::instance().bind(win);

    const size_t vertices_size = sizeof(vertices);
    const size_t indices_size = sizeof(indices);

    auto program = opengl::create_program(
        fs::path("./vec3pos_vec2tex_mat4mod.vert"),
        fs::path("./vec2tex_samp2D.frag")
    );
    std::cout << opengl::get_program_interface(program) << std::endl;

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices,
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices,
                     GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    GLuint instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * model_matrices.size(),
                 model_matrices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    for (unsigned int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(2 + i);
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                              (void*)(sizeof(glm::vec4) * i));
        glVertexAttribDivisor(2 + i, 1);
    }
    glBindVertexArray(0);

    glm::mat4 proj(1.0), view(1.0);
    const opengl::TextureData tex_data {
        .id         = opengl::gen_texture(GL_TEXTURE_2D),
        .target     = GL_TEXTURE_2D,
        .w          = 1,
        .h          = 1,
        .format     = GL_RGBA,
        .type       = GL_UNSIGNED_BYTE,
        .wrap_s     = GL_CLAMP_TO_EDGE,
        .wrap_t     = GL_CLAMP_TO_EDGE,
        .min_filter = GL_LINEAR,
        .mag_filter = GL_LINEAR
    };

    const opengl::TextureActivationCommand tex_activation {
        .tex_unit     = GL_TEXTURE0,
        .sampler_type = tex_data.target,
        .id           = tex_data.id,
        .program      = program,
        .sampler_name = "texture_1"
    };

    glm::u8vec4 color {255, 0, 0, 255};
    opengl::set_texture_meta(glm::value_ptr(color), tex_data);

    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();

        opengl::Context::instance().draw_background();
        // Активируем программу шейдеров
        glUseProgram(program);
        // Привязываем текстуру
        opengl::activate_texture(tex_activation);
        // Отправляем униформы в шейдер
        GLint projLoc = glGetUniformLocation(program, "projection");
        GLint viewLoc = glGetUniformLocation(program, "view");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // Привязываем VAO
        glBindVertexArray(VAO);
        // Рендерим инстансы
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, SQUARES);
        // Отвязываем VAO
        glBindVertexArray(0);

        glfwSwapBuffers(win);
    }

    return 0;
}
