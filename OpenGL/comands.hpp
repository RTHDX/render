#pragma once

#include <glad/glad.h>

namespace opengl {

template <typename T> struct VertexAttribCommand {
    GLuint index;
    GLuint stride;
    void* offset;

    size_t width = sizeof(T);
};

struct TextureBindCommand {
    GLenum target;
    GLenum feature;
    GLenum value;
};

struct DrawArrayCommand {
    GLuint vao;
    size_t count;
    size_t first = 0;
    GLenum mode = GL_TRIANGLES;
};

}
