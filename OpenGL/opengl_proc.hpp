#pragma once

#include <vector>
#include <filesystem>
#include <string_view>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "comands.hpp"
#include "opengl_context.hpp"


namespace opengl {
using byte_t = unsigned char;
using stencil_idx_t = GLubyte;


void on_gl_error(GLenum error_code, const char* call, const char* file,
                 int line);
#define SAFE_CALL(gl_call)\
    gl_call;\
    opengl::on_gl_error(glGetError(), #gl_call, __FILE__, __LINE__);

bool check_shader(GLuint id);
bool check_program(GLuint id);

void viewport(GLsizei width, GLsizei height);
void viewport(GLsizei x, GLsizei y, GLsizei w, GLsizei h);
void background(const glm::vec4 color, GLbitfield clear_bits);


std::vector<GLuint> gen_vertex_array(size_t count);
GLuint gen_vertex_array();
void free_vertex_array(const std::vector<GLuint>& in);
void free_vertex_array(GLuint id);

std::vector<GLuint> gen_element_buffers(size_t count);
GLuint gen_element_buffer();
void free_element_buffers(std::vector<GLuint>& in);
void free_element_buffer(GLuint id);

std::vector<GLuint> gen_vertex_buffers(size_t count);
GLuint gen_vertex_buffers();
void free_vertex_buffers(const std::vector<GLuint>& in);
void free_vertex_buffer(GLuint id);

std::vector<GLuint> gen_pixel_buffers(size_t count);
GLuint gen_pixel_buffers();
void free_pixel_buffers(const std::vector<GLuint>& id);
void free_pixel_buffer(GLuint id);

std::vector<GLuint> gen_framebuffers(size_t count);
GLuint gen_framebuffer();
void free_framebuffers(const std::vector<GLuint>& ids);
void free_framebuffer(GLuint* id);

template <typename T>
T read_pixel(GLint x, GLint y, GLenum t, GLenum f, T* d) {
    SAFE_CALL(glReadPixels(x, y, 1, 1, t, f, (void*)d));
    return *d;
}

glm::u8vec4 read_pixel_color(GLint x, GLint y);
stencil_idx_t read_stencil(GLint x, GLint y);

template <typename T>
inline void bind_vbo(GLuint id, const std::vector<T>& in) {
    assert(Context::instance().bound_vao() > 0);
    const T* data = in.data();
    const size_t width = in.size() * sizeof (T);
    SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));
    SAFE_CALL(glBufferData(GL_ARRAY_BUFFER, width, data, GL_STATIC_DRAW));
}

inline void bind_ebo(GLuint id, const std::vector<GLuint>& in) {
    assert(Context::instance().bound_vao() > 0);
    const auto* data = in.data();
    const size_t width = in.size() * sizeof(GLuint);
    SAFE_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
    SAFE_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, width, data, GL_STATIC_DRAW));
}

template <typename T>
inline void bind_pbo(GLuint id, const std::vector<T>& in) {
    const T* data = in.data();
    const size_t width = in.size() * sizeof (T);
    SAFE_CALL(glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, id));
    SAFE_CALL(glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, width, data,
                           GL_DYNAMIC_DRAW));
}

void bind_vao(GLuint id);
void bind_fbo(GLuint id);

template<typename T>
inline void set_vertex_attrib(const vertex_attrib_command_t<T>& cmd) {
    SAFE_CALL(glEnableVertexAttribArray(cmd.index));
    SAFE_CALL(glVertexAttribPointer(cmd.index, cmd.stride, GL_FLOAT, GL_FALSE,
                                    cmd.width, cmd.offset));
}


template <typename Iterable>
inline void do_vertex_attrib_cmds(Iterable&& comands) {
    assert(Context::instance().bound_vao() > 0);

    for (const auto& cmd : comands) {
        SAFE_CALL(glEnableVertexAttribArray(cmd.index));
        SAFE_CALL(glVertexAttribPointer(cmd.index, cmd.stride, GL_FLOAT,
                                        GL_FALSE, cmd.width, cmd.offset));
    }
}

GLuint gen_texture(GLenum target = GL_TEXTURE_2D);
void activate_texture(const texture_activation_command_t& cmd);
void free_texture(GLuint id);

void apply_stencil(stencil_command_t&& cmd);

void use(GLuint id);

void draw(const draw_array_command_t& cmd);
void draw(const draw_elements_command_t& cmd);
void draw_array_framebuffer(const draw_array_fbuff_t& cmd);
void draw_instance_array(const draw_array_instanced_t& cmd);
void draw_instance_elements(const draw_elements_instanced_t& cmd);

bool set_vec3(GLuint id, const std::string_view name, const glm::vec3& val);
bool set_vec4(GLuint id, const std::string_view name, const glm::vec4& val);
bool set_mat4(GLuint id, const std::string_view name, const glm::mat4& val);
bool set_vec2(GLuint id, const std::string_view name, const glm::vec2& val);
bool set_int(GLuint id, const std::string_view name, GLint value);
bool set_float(GLuint id, const std::string_view name, GLfloat value);

struct ShaderProgramInterface final {
    GLint uniforms_count = 0;
    std::unordered_map<std::string, GLenum> uniform_block;
    GLint input_count = 0;
    std::unordered_map<std::string, GLenum> input_block;

    std::optional<std::string> found_uname_by_value(GLenum) const;
    std::optional<std::string> found_iname_by_balue(GLenum) const;
};
std::ostream& operator << (std::ostream& os, const ShaderProgramInterface& i);

ShaderProgramInterface get_program_interface(GLuint program);

struct program_bind_guard_t final {
    program_bind_guard_t(GLuint id) { opengl::use(id); }
    ~program_bind_guard_t()         { opengl::use(0); }

    program_bind_guard_t(const program_bind_guard_t&)              = delete;
    program_bind_guard_t& operator = (const program_bind_guard_t&) = delete;
    program_bind_guard_t(program_bind_guard_t&&)                   = delete;
    program_bind_guard_t& operator = (program_bind_guard_t&&)      = delete;
};

struct buffer_bind_guard final {
    buffer_bind_guard() = delete;
    buffer_bind_guard(const buffer_bind_guard&) = delete;
    buffer_bind_guard& operator = (const buffer_bind_guard&) = delete;
    buffer_bind_guard(buffer_bind_guard&&) = delete;
    buffer_bind_guard& operator = (buffer_bind_guard&&) = delete;

    buffer_bind_guard(vao_bind_command_t&& cmd);
    buffer_bind_guard(buff_bind_command_t&& cmd);
    ~buffer_bind_guard();

private:
    enum __buffer_bind_mode__ {
        VAO_BINDER,
        VBO_BINDER
    } mode_;
    GLenum buff_type_ = 0;
};

}
