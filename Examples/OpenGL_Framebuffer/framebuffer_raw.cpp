#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


int main() {
    GLuint fbo_1, tex_1;
    GLsizei lastFboWidth = 0, lastFboHeight = 0;

    // Генерация FBO
    glGenFramebuffers(1, &fbo_1);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_1);

    // Генерация текстуры
    glGenTextures(1, &tex_1);
    glBindTexture(GL_TEXTURE_2D, tex_1);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           tex_1, 0);

    // Проверка, все ли хорошо с нашим FBO
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "FBO_1 not completed" << std::endl;
        return EXIT_FAILURE;
    }
    // Вернемся к обычному FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return 0;
}