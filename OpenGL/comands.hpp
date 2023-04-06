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

struct StencilCommand {
    // GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_INCR_WRAP, GL_DECR,
    // GL_DECR_WRAP, GL_INVERT
    GLenum s_fail;
    GLenum dp_fail;
    GLenum dp_pass;
    // GL_NEVER, GL_LESS, GL_LEQUAL, GL_GREATER, GL_GEQUAL, GL_EQUAL,
    // GL_NOTEQUAL, GL_ALWAYS
    GLenum function;
    GLint ref;
    GLuint mask;
};

struct TextureActivationCommand {
    GLenum tex_type;
    GLenum sampler_type;
    GLuint id;
    GLuint program;
    std::string_view sampler_name;
};

struct DrawArrayCommand {
    GLuint vao;
    size_t count;
    size_t first = 0;
    GLenum mode = GL_TRIANGLES;
};

struct DrawElementsCommand {
    GLuint vao;
    size_t count;
    size_t* indices = nullptr;
    GLenum type = GL_UNSIGNED_INT;
    GLenum mode = GL_TRIANGLES;
};

}
