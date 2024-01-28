#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "opengl_proc.hpp"
#include "comands.hpp"

namespace opengl {
using buffers_t = std::vector<GLuint>;
using elements_input_t = std::vector<GLuint>;

struct vec2pos {
    using vertex_attrib_t = VertexAttribCommand<vec2pos>;
    using commands_t = std::vector<vertex_attrib_t>;
    using this_t = vec2pos;
    using vertex_input_t = std::vector<this_t>;

    glm::vec2 pos;

public:
    vec2pos() = default;
    vec2pos(glm::vec2&& pos);

    static buffers_t gen_buffers(GLuint vao, const vertex_input_t& in);
    static buffers_t gen_buffers(GLuint vao, const vertex_input_t& in,
                                 GLuint ebo, const elements_input_t& ebo_vs);
    static commands_t commands();
};

struct vec3pos {
    using vertex_attrib_t = VertexAttribCommand<vec3pos>;
    using commands_t = std::vector<vertex_attrib_t>;
    using this_t = vec3pos;
    using vertex_input_t = std::vector<this_t>;

    glm::vec3 pos;

public:
    vec3pos() = default;
    vec3pos(glm::vec3&& p);

    static buffers_t gen_buffers(GLuint vao,
                                 const std::vector<this_t>& in);
    static buffers_t gen_buffers(GLuint vao, const vertex_input_t& in,
                                 GLuint ebo, const elements_input_t& ebo_vs);
    static commands_t commands();
};

struct vec3pos_vec3norm_t {
    using vertex_attrib_t = VertexAttribCommand<vec3pos_vec3norm_t>;
    using commands_t = std::vector<vertex_attrib_t>;
    using this_t = vec3pos_vec3norm_t;
    using vertex_input_t = std::vector<this_t>;

    glm::vec3 pos;
    glm::vec3 normal;

public:
    vec3pos_vec3norm_t() = default;
    vec3pos_vec3norm_t(glm::vec3&& p, glm::vec3&& n);

    static buffers_t gen_buffers(GLuint vao, const vertex_input_t& in);
    static buffers_t gen_buffers(GLuint vao, const vertex_input_t& in,
                                 GLuint ebo, const elements_input_t& ebo_vs);
    static commands_t commands();
};


struct vec3pos_vec3norm_vec2tex_t {
    using vertex_attrib_t = VertexAttribCommand<vec3pos_vec3norm_vec2tex_t>;
    using commands_t = std::vector<vertex_attrib_t>;
    using this_t = vec3pos_vec3norm_vec2tex_t;
    using vertex_input_t = std::vector<this_t>;

    glm::vec3 pos;
    glm::vec3 norm;
    glm::vec2 tex_pos;

public:
    vec3pos_vec3norm_vec2tex_t() = default;
    vec3pos_vec3norm_vec2tex_t(glm::vec3&& p, glm::vec3&& n, glm::vec2&& t);

    static buffers_t gen_buffers(GLuint vao,
                                 const std::vector<this_t>& in);
    static commands_t commands();
};


struct vec3pos_vec2tex_t {
    using vertex_attrib_t = VertexAttribCommand<vec3pos_vec2tex_t>;
    using commands_t = std::vector<vertex_attrib_t>;
    using this_t = vec3pos_vec2tex_t;
    using vertex_input_t = std::vector<this_t>;

    glm::vec3 pos;
    glm::vec2 tex_pos;

public:
    vec3pos_vec2tex_t() = default;
    vec3pos_vec2tex_t(glm::vec3&& pos, glm::vec2&& t);

    static buffers_t gen_buffers(GLuint vao,
                                 const std::vector<this_t>& in);
    static buffers_t gen_buffers(GLuint vao, const std::vector<this_t>& in,
                                 GLuint ebo, const std::vector<GLuint>& ebo_v);
    static commands_t commands();
};


struct mat4_instanced final {
    using this_t = mat4_instanced;
    using col_t = glm::vec4;

    glm::mat4 mat;

public:
    mat4_instanced() = default;
    mat4_instanced(glm::mat4&& m);

    static std::vector<mat4_instanced> convert(const std::vector<glm::mat4>&);
    static GLuint gen_buffer(GLuint vao,
                             const std::vector<this_t>& in,
                             GLuint index,
                             GLenum usage = GL_STATIC_DRAW);
};


struct float_instanced final {
    using this_t = float_instanced;
    using col_t = float;

    float val;

public:
    float_instanced() = default;
    float_instanced(float v);

    static std::vector<float_instanced> convert(const std::vector<float>& in);
    static GLuint gen_buffer(GLuint vao,
                             const std::vector<this_t>& in,
                             GLuint index,
                             GLenum usage = GL_STATIC_DRAW);
    static void update(GLuint id,
                       const std::vector<this_t>& in,
                       size_t offset = 0);
};


struct mat4_f_instanced final {
    using this_t = mat4_f_instanced;
    using col_t = glm::vec4;

    glm::mat4 mat;
    float val;

public:
    mat4_f_instanced() = default;
    mat4_f_instanced(glm::mat4&& mat, float val);

    static std::vector<mat4_f_instanced> zip(
        const std::vector<glm::mat4>& matrices,
        const std::vector<float>& floats
    );
    static GLuint gen_buffer(GLuint vao,
                             const std::vector<this_t>& in,
                             GLuint index,
                             GLenum usage = GL_STATIC_DRAW);
};


template <typename T> concept vertex_input_c =
    requires (T t) {
        typename T::vertex_attrib_t;
        typename T::commands_t;
        typename T::this_t;
        typename T::vertex_input_t;
    };

template <typename T> concept instanced_input_c =
    requires (T t) {
        typename T::this_t;
        typename T::col_t;
    };
}
