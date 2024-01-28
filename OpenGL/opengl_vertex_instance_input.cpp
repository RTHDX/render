#include "opengl_vertex_input.hpp"


namespace opengl {

template<typename T> static GLuint
generate_abo(const std::vector<T>& in, GLenum usage) {
    static constexpr size_t SIZE = sizeof (T);
    
    GLuint abo;
    SAFE_CALL(glGenBuffers(1, &abo));
    SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, abo));
    SAFE_CALL(glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(T) * in.size(),
        in.data(),
        usage
    ));
    SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    return abo;
}

static void layout_mat4(GLuint index, GLsizei total_size,
                        GLsizei offset = sizeof(glm::vec4)) {
    for (GLuint i = 0; i < 4; i++) {
        SAFE_CALL(glEnableVertexAttribArray(index + i));
        SAFE_CALL(glVertexAttribPointer(
            index + i,
            4, // 4 floats
            GL_FLOAT,
            GL_FALSE,
            total_size,
            (void*)(offset * i)
        ));
        SAFE_CALL(glVertexAttribDivisor(index + i, 1));
    }
}

static void layout_float(GLuint index, GLuint total_size,
                         GLsizei offset = sizeof(float)) {
    SAFE_CALL(glEnableVertexAttribArray(index));
    SAFE_CALL(glVertexAttribPointer(
        index,
        1, // 1 float
        GL_FLOAT,
        GL_FALSE,
        total_size,
        (void*)offset
    ));
    SAFE_CALL(glVertexAttribDivisor(index, 1));
}


mat4_instanced::mat4_instanced(glm::mat4&& m)
    : mat(std::move(m))
{}

std::vector<mat4_instanced>
mat4_instanced::convert(const std::vector<glm::mat4>& in) {
    std::vector<this_t> out(in.size());
    for (size_t i = 0; i < in.size(); ++i) {
        out[i].mat = in[i];
    }
    return out;
}

GLuint mat4_instanced::gen_buffer(GLuint vao,
                                  const std::vector<this_t>& in,
                                  GLuint index,
                                  GLenum usage) {
    static constexpr GLuint SIZE_OF_THIS = sizeof(this_t);

    GLuint abo = generate_abo(in, usage);
    buffer_bind_guard vao_lock({.vao = vao});
    buffer_bind_guard abo_lock({.id = abo, .type = GL_ARRAY_BUFFER});
    layout_mat4(index, sizeof(this_t));
    return abo;
}


float_instanced::float_instanced(float v)
    : val(v)
{}

std::vector<float_instanced>
float_instanced::convert(const std::vector<float>& in) {
    std::vector<this_t> out(in.size());
    for (size_t i = 0; i < in.size(); ++i) {
        out[i].val = in[i];
    }
    return out;
}

GLuint float_instanced::gen_buffer(GLuint vao,
                                   const std::vector<this_t>& in,
                                   GLuint index,
                                   GLenum usage) {
    GLuint abo = generate_abo(in, usage);
    buffer_bind_guard vao_lock({.vao = vao});
    buffer_bind_guard abo_lock({.id = abo, .type = GL_ARRAY_BUFFER});
    layout_float(index, sizeof (this_t), 0);
    return abo;
}

void float_instanced::update(GLuint id,
                             const std::vector<this_t>& frames,
                             size_t offset) {
    buffer_bind_guard abo_bind({.id = id, .type = GL_ARRAY_BUFFER});
    SAFE_CALL(glBufferSubData(
        GL_ARRAY_BUFFER,
        offset,
        frames.size() * sizeof(this_t),
        frames.data()
    ));
}

mat4_f_instanced::mat4_f_instanced(glm::mat4&& m, float v)
    : mat(std::move(m))
    , val(v)
{}

std::vector<mat4_f_instanced>
mat4_f_instanced::zip(const std::vector<glm::mat4>& matrices,
                      const std::vector<float>& floats) {
    if (matrices.size() != floats.size()) {
        throw std::runtime_error("matrices.size() != floats.size()");
    }

    const size_t len = matrices.size();
    std::vector<mat4_f_instanced> out(len);
    for (size_t i = 0; i < len; ++i) {
        out[i].mat = matrices[i];
        out[i].val = floats[i];
    }
    return out;
}

GLuint mat4_f_instanced::gen_buffer(GLuint vao,
                                    const std::vector<this_t>& in,
                                    GLuint index,
                                    GLenum usage) {
    static constexpr GLuint SIZE_OF_THIS = sizeof(this_t);

    GLuint abo = generate_abo(in, usage);
    buffer_bind_guard vao_lock({.vao = vao});
    buffer_bind_guard abo_lock({.id = abo, .type = GL_ARRAY_BUFFER});
    layout_mat4(index, SIZE_OF_THIS);
    layout_float(index + 4, SIZE_OF_THIS, sizeof(glm::mat4));
    return abo;
}

}
