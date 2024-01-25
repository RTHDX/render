#include "opengl_vertex_input.hpp"


namespace opengl {

template <vertex_input_c I>
buffers_t generic_gen_buffers(GLuint vao,
                              const typename I::vertex_input_t& input) {
    auto buffers = gen_vertex_buffers(1);
    bind_vao(vao);
    for (GLuint id : buffers) {
        bind_vbo<I>(id, input);
    }
    do_vertex_attrib_cmds(std::move(I::commands()));
    bind_vao(0);
    return buffers;
}

template <vertex_input_c I>
buffers_t generic_gen_buffers(GLuint vao,
                              const typename I::vertex_input_t& input,
                              GLuint ebo,
                              const std::vector<GLuint>& ebo_v) {
    auto buffers = gen_vertex_buffers(1);
    bind_vao(vao);
    for (GLuint id : buffers) {
        bind_vbo<I>(id, input);
    }
    bind_ebo(ebo, ebo_v);
    do_vertex_attrib_cmds(std::move(I::commands()));
    bind_vao(0);
    return buffers;
}

vec2pos::vec2pos(glm::vec2&& pos)
    : pos(std::move(pos))
{}

buffers_t vec2pos::gen_buffers(GLuint vao,
                               const vertex_input_t& in) {
    return generic_gen_buffers<this_t>(vao, in);
}

buffers_t vec2pos::gen_buffers(GLuint vao, const vertex_input_t& in,
                               GLuint ebo, const std::vector<GLuint>& ebo_v) {
    return generic_gen_buffers<this_t>(vao, in, ebo, ebo_v);
}

vec2pos::commands_t vec2pos::commands() {
    return {
        {.index = 0, .stride=2, .offset=(void*)offsetof(this_t, pos)}
    };
}


vec3pos::vec3pos(glm::vec3&& p)
    : pos(std::move(p))
{}

buffers_t vec3pos::gen_buffers(GLuint vao,
                               const std::vector<vec3pos>& in) {
    auto buffers = gen_vertex_buffers(1);
    bind_vao(vao);
    for (GLuint id : buffers) {
        bind_vbo<this_t>(id, in);
    }
    do_vertex_attrib_cmds(std::move(this_t::commands()));
    bind_vao(0);
    return buffers;
}

buffers_t vec3pos::gen_buffers(GLuint vao, const vertex_input_t& in,
                               GLuint ebo, const elements_input_t& ebo_v) {
    return generic_gen_buffers<this_t>(vao, in, ebo, ebo_v);
}

vec3pos::commands_t vec3pos::commands() {
    return {
        {.index=0, .stride=3, .offset=(void*)offsetof(this_t, pos)}
    };
}

vec3pos_vec3norm_t::vec3pos_vec3norm_t(glm::vec3&& p, glm::vec3&& n)
    : pos(std::move(p))
    , normal(std::move(n))
{}

buffers_t vec3pos_vec3norm_t::gen_buffers(GLuint vao,
                                          const std::vector<this_t>& in) {
    auto out = gen_vertex_buffers(2);
    bind_vao(vao);
    for (GLuint id : out) {
        bind_vbo<this_t>(id, in);
    }
    do_vertex_attrib_cmds(std::move(this_t::commands()));
    bind_vao(0);
    return out;
}

vec3pos_vec3norm_t::commands_t vec3pos_vec3norm_t::commands() {
    return {
        {.index=0, .stride=3, .offset=(void*)offsetof(this_t, pos)},
        {.index=1, .stride=3, .offset=(void*)offsetof(this_t, normal)}
    };
}


vec3pos_vec3norm_vec2tex_t::vec3pos_vec3norm_vec2tex_t(glm::vec3&& p,
                                                       glm::vec3&& n,
                                                       glm::vec2&& t)
    : pos(std::move(p))
    , norm(std::move(n))
    , tex_pos(std::move(t))
{}

buffers_t vec3pos_vec3norm_vec2tex_t::gen_buffers(GLuint vao,
                                                  const vertex_input_t& in) {
    auto buffers = gen_vertex_buffers(1);
    bind_vao(vao);
    for (GLuint id : buffers) {
        bind_vbo<this_t>(id, in);
    }
    do_vertex_attrib_cmds(std::move(this_t::commands()));
    bind_vao(0);
    return buffers;
}

vec3pos_vec3norm_vec2tex_t::commands_t
vec3pos_vec3norm_vec2tex_t::commands() {
    return {
        {.index=0, .stride=3, .offset=(void*)offsetof(this_t, pos)},
        {.index=1, .stride=3, .offset=(void*)offsetof(this_t, norm)},
        {.index=2, .stride=2, .offset=(void*)offsetof(this_t, tex_pos)}
    };
}


vec3pos_vec2tex_t::vec3pos_vec2tex_t(glm::vec3&& p, glm::vec2&& t)
    : pos(std::move(p))
    , tex_pos(std::move(t))
{}

buffers_t vec3pos_vec2tex_t::gen_buffers(GLuint vao,
                                         const std::vector<this_t>& in) {
    auto buffers = gen_vertex_buffers(1);
    bind_vao(vao);
    for (GLuint id : buffers) {
        bind_vbo<this_t>(id, in);
    }
    do_vertex_attrib_cmds(std::move(this_t::commands()));
    bind_vao(0);
    return buffers;
}

buffers_t vec3pos_vec2tex_t::gen_buffers(GLuint vao,
                                         const std::vector<this_t>& in,
                                         GLuint ebo,
                                         const std::vector<GLuint>& ebo_v) {
    auto buffers = gen_vertex_buffers(1);
    bind_vao(vao);
    for (GLuint id : buffers) {
        bind_vbo<this_t>(id, in);
    }
    bind_ebo(ebo, ebo_v);
    do_vertex_attrib_cmds(std::move(this_t::commands()));
    bind_vao(0);
    return buffers;
}

vec3pos_vec2tex_t::commands_t vec3pos_vec2tex_t::commands() {
    return {
        {.index=0, .stride=3, .offset=(void*)offsetof(this_t, pos)},
        {.index=1, .stride=2, .offset=(void*)offsetof(this_t, tex_pos)}
    };
}


mat4_instanced::mat4_instanced(glm::mat4&& m)
    : mat(std::move(m))
{}

GLuint mat4_instanced::gen_buffer(GLuint vao,
                                  const std::vector<this_t>& in,
                                  GLuint index) {
    static constexpr GLuint STRIDE = 4;
    static constexpr GLuint SIZE_OF_THIS = sizeof (this_t);
    static constexpr GLuint SIZE_OF_COL = sizeof (this_t::col_t);

    GLuint vbo;
    SAFE_CALL(glGenBuffers(1, &vbo));
    SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    SAFE_CALL(glBufferData(
        GL_ARRAY_BUFFER,
        SIZE_OF_THIS * in.size(),
        in.data(),
        GL_STATIC_DRAW
    ));
    SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    SAFE_CALL(glBindVertexArray(vao));
    SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    for (unsigned int i = 0; i < 4; i++) {
        SAFE_CALL(glEnableVertexAttribArray(index + i));
        SAFE_CALL(glVertexAttribPointer(
            index + i,
            STRIDE,
            GL_FLOAT,
            GL_FALSE,
            SIZE_OF_THIS,
            (void*)(SIZE_OF_COL * i)
        ));
        SAFE_CALL(glVertexAttribDivisor(index + i, 1));
    }
    SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    SAFE_CALL(glBindVertexArray(0));
    return vbo;
}

mat4_f_instanced::mat4_f_instanced(glm::mat4&& m, float v)
    : mat(std::move(m))
    , val(v)
{}

GLuint mat4_f_instanced::gen_buffer(GLuint vao,
                                    const std::vector<this_t>& in,
                                    GLuint index) {
    static constexpr GLuint STRIDE = 4;
    static constexpr GLuint SIZE_OF_THIS = sizeof(this_t);
    static constexpr GLuint SIZE_OF_MAT = sizeof(glm::mat4);
    static constexpr GLuint SIZE_OF_FLOAT = sizeof(float);

    GLuint vbo;
    SAFE_CALL(glGenBuffers(1, &vbo));
    SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    SAFE_CALL(glBufferData(
        GL_ARRAY_BUFFER,
        SIZE_OF_THIS * in.size(),
        in.data(),
        GL_STATIC_DRAW
    ));

    SAFE_CALL(glBindVertexArray(vao));
    SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    for (unsigned int i = 0; i < 4; i++) {
        SAFE_CALL(glEnableVertexAttribArray(index + i));
        SAFE_CALL(glVertexAttribPointer(
            index + i,
            STRIDE,
            GL_FLOAT,
            GL_FALSE,
            SIZE_OF_THIS,
            (void*)(sizeof(glm::vec4) * i)
        ));
        SAFE_CALL(glVertexAttribDivisor(index + i, 1));
    }
    SAFE_CALL(glEnableVertexAttribArray(index + 4));
    SAFE_CALL(glVertexAttribPointer(
        index + 4,
        1, // Only one float val
        GL_FLOAT,
        GL_FALSE,
        SIZE_OF_THIS,
        (void*)SIZE_OF_MAT // Offset after mat4
    ));
    SAFE_CALL(glVertexAttribDivisor(index + 4, 1));

    SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    SAFE_CALL(glBindVertexArray(0));
    return vbo;
}


}
