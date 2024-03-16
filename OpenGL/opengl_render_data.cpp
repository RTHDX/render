#include "opengl_proc.hpp"
#include "opengl_utils.hpp"
#include "opengl_render_data.hpp"


namespace opengl {

GLuint create_program(const std::string& vertex_shader_src,
    const std::string& fragment_shader_src) {
    GLuint program = glCreateProgram(),
           vertex_shader = glCreateShader(GL_VERTEX_SHADER),
           fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    const auto* src = vertex_shader_src.data();
    SAFE_CALL(glShaderSource(vertex_shader, 1, &src, nullptr));
    SAFE_CALL(glCompileShader(vertex_shader));
    if (!check_shader(vertex_shader)) {
        SAFE_CALL(glDeleteShader(vertex_shader));
    }

    src = fragment_shader_src.data();
    SAFE_CALL(glShaderSource(fragment_shader, 1, &src, nullptr));
    SAFE_CALL(glCompileShader(fragment_shader));
    if (!check_shader(fragment_shader)) {
        SAFE_CALL(glDeleteShader(fragment_shader));
    }

    SAFE_CALL(glAttachShader(program, vertex_shader));
    SAFE_CALL(glAttachShader(program, fragment_shader));
    SAFE_CALL(glLinkProgram(program));
    if (!check_program(program)) {
        SAFE_CALL(glDeleteProgram(program));
    }
    SAFE_CALL(glDetachShader(program, vertex_shader));
    SAFE_CALL(glDetachShader(program, fragment_shader));
    return program;
}

GLuint create_program(const std::filesystem::path& vertex_path,
    const std::filesystem::path& fragment_path) {
    auto vertex_src = opengl::utils::read_shader(vertex_path);
    auto fragment_src = opengl::utils::read_shader(fragment_path);
    return create_program(vertex_src, fragment_src);
}

void free_program(GLuint id) {
    SAFE_CALL(glDeleteProgram(id));
}

void render_data_t::free() {
    if (Context::instance().is_context_active()) {
        SAFE_CALL(glDeleteBuffers(1, &ebo));
        SAFE_CALL(glDeleteBuffers(vertex_buffers.size(),
                                  vertex_buffers.data()));
        SAFE_CALL(glDeleteVertexArrays(1, &vao));
        SAFE_CALL(glDeleteProgram(program));
    }
    ebo = 0;
    vertex_buffers.clear();
    vao = 0;
    stencil_ref = -1;
    ebo_count = 0;
}

}
