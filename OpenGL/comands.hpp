#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glad/glad.h>


namespace opengl {

template <typename T> struct vertex_attrib_command_t final {
    GLuint index;
    GLuint stride;
    void* offset;

    size_t width = sizeof(T);
};


struct texture_bind_command_t final {
    GLenum target;
    GLenum feature;
    GLenum value;
};


struct stencil_command_t final {
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


struct texture_activation_command_t final {
    GLenum tex_unit;     // GL_TEXTURE0, GL_TEXTURE_1, ...
    GLenum sampler_type; // GL_TEXTURE_2D, GL_TEXTURE_2D_ARRAY, ...
    GLuint id;           // texure_id
    GLuint program;
    std::string sampler_name;
};


struct draw_array_command_t final {
    GLuint vao;
    size_t count;
    size_t first {0};
    GLenum mode  {GL_TRIANGLES};
};


struct draw_elements_command_t final {
    GLuint vao;
    GLsizei count;
    size_t* indices {nullptr};
    GLenum type     {GL_UNSIGNED_INT};
    GLenum mode     {GL_TRIANGLES};
};


struct draw_array_fbuff_t final {
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


struct fbuff_render_ctx_t final {
    GLuint fbo;
    glm::ivec4 viewport;
    glm::ivec4 screen_viewport;
    glm::vec4  background;
    GLbitfield clear_bits {GL_COLOR_BUFFER_BIT};
};


struct draw_array_instanced_t final {
    GLuint vao;
    GLsizei count;
    GLsizei instancecount;
    GLint first {0};
    GLenum mode {GL_TRIANGLES};
};


struct draw_elements_instanced_t final {
    GLuint vao;
    GLsizei count;
    GLsizei instancecount;
    GLuint* indices {nullptr};
    GLenum type     {GL_UNSIGNED_INT};
    GLenum mode     {GL_TRIANGLES};
};

struct draw_elements_instanced_fbuff_t final {
    draw_elements_instanced_t draw_cmd;
    fbuff_render_ctx_t fb_ctx;
};


struct vao_bind_command_t final {
    GLuint vao;
};

struct buff_bind_command_t final {
    GLuint id;
    GLenum type;
};

struct buff_gen_command_t final {
    GLenum type; // GL_ARRAY_BUFFER
};

}
