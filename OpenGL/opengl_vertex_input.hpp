#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "opengl_proc.hpp"
#include "comands.hpp"

namespace opengl {
using buffers_t = std::vector<GLuint>;

struct vec3pos {
    using vertex_attrib_t = VertexAttribCommand<vec3pos>;
    using commands_t = std::vector<vertex_attrib_t>;
    using this_t = vec3pos;

    glm::vec3 pos;

public:
    vec3pos() = default;
    vec3pos(glm::vec3&& p);

    static buffers_t gen_buffers(GLuint vao,
                                 const std::vector<this_t>& in);
    static commands_t commands();
};

struct vec3pos_vec3norm_t {
    using vertex_attrib_t = VertexAttribCommand<vec3pos_vec3norm_t>;
    using commands_t = std::vector<vertex_attrib_t>;
    using this_t = vec3pos_vec3norm_t;

    glm::vec3 pos;
    glm::vec3 normal;

public:
    vec3pos_vec3norm_t() = default;
    vec3pos_vec3norm_t(glm::vec3&& p, glm::vec3&& n);

    static buffers_t gen_buffers(GLuint vao,
                                 const std::vector<this_t>& in);
    static commands_t commands();
};


struct vec3pos_vec3norm_vec2tex_t {
    using vertex_attrib_t = VertexAttribCommand<vec3pos_vec3norm_vec2tex_t>;
    using commands_t = std::vector<vertex_attrib_t>;
    using this_t = vec3pos_vec3norm_vec2tex_t;
    using this_in_t = std::vector<this_t>;

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

    glm::vec3 pos;
    glm::vec2 tex_pos;

public:
    vec3pos_vec2tex_t() = default;
    vec3pos_vec2tex_t(glm::vec3&& pos, glm::vec2&& t);

    static buffers_t gen_buffers(GLuint vao,
                                 const std::vector<this_t>& in);
    static commands_t commands();
};

}