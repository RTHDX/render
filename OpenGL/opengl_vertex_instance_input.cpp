#include "opengl_vertex_input.hpp"


namespace opengl {

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

GLuint mat4_instanced::gen_buffer(GLuint vao,
                                  const std::vector<this_t>& in,
                                  GLuint index) {
    static constexpr GLuint SIZE_OF_THIS = sizeof(this_t);

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
    layout_mat4(index, sizeof(this_t));
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
    static constexpr GLuint SIZE_OF_THIS = sizeof(this_t);

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

    layout_mat4(index, SIZE_OF_THIS);
    layout_float(index + 4, SIZE_OF_THIS, sizeof(glm::mat4));

    SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    SAFE_CALL(glBindVertexArray(0));
    return vbo;
}

}
