#pragma once

#include <glad/glad.h>

#include "texture.hpp"
#include "comands.hpp"


namespace opengl {

struct FramebufferData final {
    GLuint fbo;
    GLenum attachment_point; // GL_COLOR_ATTACHMENT0 ... GL_COLOR_ATTACHMENT31
    TextureData texture;
    GLenum target {GL_FRAMEBUFFER};

public:
    static FramebufferData create(int width, int height);

    GLenum status() const;
    void free();
};

}
