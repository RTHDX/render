#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>


namespace opengl {

template <typename T> struct VertexAttribCommand final {
    GLuint index;
    GLuint stride;
    void* offset;

    size_t width = sizeof(T);
};


struct TextureBindCommand final {
    GLenum target;
    GLenum feature;
    GLenum value;
};


struct StencilCommand final {
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


struct TextureActivationCommand final {
    GLenum tex_type;
    GLenum sampler_type;
    GLuint id;
    GLuint program;
    std::string sampler_name;
};


struct DrawArrayCommand final {
    GLuint vao;
    size_t count;
    size_t first {0};
    GLenum mode  {GL_TRIANGLES};
};


struct DrawElementsCommand final {
    GLuint vao;
    size_t count;
    size_t* indices {nullptr};
    GLenum type     {GL_UNSIGNED_INT};
    GLenum mode     {GL_TRIANGLES};
};


struct DrawArrayFramebuffer final {
    GLuint fbo;
    GLuint vao;
    size_t count;
    glm::ivec4 viewport;
    glm::ivec4 screen_viewport;
    glm::vec4 background;
    GLbitfield clear_bits {GL_COLOR_BUFFER_BIT};
    size_t first          {0};
    GLenum mode           {GL_TRIANGLES};
};


struct DrawArrayInstanced final {
    GLuint vao;
    GLsizei count;
    GLsizei instancecount;
    GLint first {0};
    GLenum mode {GL_TRIANGLES};
};


struct DrawElementInstanced final {
    GLuint vao;
    GLsizei count;
    GLsizei instancecount;
    GLuint* indices {nullptr};
    GLenum type     {GL_UNSIGNED_INT};
    GLenum mode     {GL_TRIANGLES};
};

}
