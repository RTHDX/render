#pragma once

#include <vector>
#include <filesystem>
#include <string_view>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "comands.hpp"

namespace opengl {
using byte_t = unsigned char;

class Context {
public:
    static Context& instance();

    void initialize(bool = false);
    void initialize_light(bool = false);
    void dump() const;

    void viewport(int width, int height) const;
    glm::vec4& background() { return _background; }
    void background(const glm::vec4& color);

    void draw_background() const;

    GLint active_program() const;
    GLint bound_vao() const;
    GLint bound_texture_2d() const;
    GLint bound_texture_2d_array() const;

private:
    Context() = default;

private:
    glm::vec4 _background;
};


void on_gl_error(GLenum error_code, const char* call, const char* file,
                 int line);
#define SAFE_CALL(gl_call)\
    gl_call;\
    opengl::on_gl_error(glGetError(), #gl_call, __FILE__, __LINE__);
//#define SAFE_CALL(call) call;


GLuint create_program(const std::filesystem::path& vertex_path,
                      const std::filesystem::path& fragment_path);
GLuint create_program(const std::string& vertex_shader,
                      const std::string& fragment_shader);
void free_program(GLuint id);

bool check_shader(GLuint id);
bool check_program(GLuint id);


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

template <typename T>
T read_pixel(GLint x, GLint y, GLenum t, GLenum f, T* d) {
    SAFE_CALL(glReadPixels(x, y, 1, 1, t, f, (void*)d));
    return *d;
}

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

template<typename T>
inline void set_vertex_attrib(const VertexAttribCommand<T>& cmd) {
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

struct TextureData {
    GLuint id;
    GLenum target;
    GLint w, h;
    GLint format; // GL_DEPTH_COMPONENT GL_DEPTH_STENCIL GL_RED GL_RG GL_RGB GL_RGBA
    GLenum type; // GL_UNSIGNED_BYTE...
    GLenum wrap_s, wrap_t, min_filter, mag_filter;
};
std::ostream& operator << (std::ostream& os, const TextureData& tex);

GLuint gen_texture(GLenum target = GL_TEXTURE_2D);
void bind_texture(const glm::ivec2& dims,
                  std::vector<glm::vec3>&& texture);
void bind_texture(const glm::ivec2& dims,
                  byte_t* texture);
void bind_texture(GLenum target, GLuint id);
void set_texture_meta(byte_t* raw_data, const TextureData& params);
void activate_texture(TextureActivationCommand&& cmd);
void free_texture(GLuint id);

void apply_stencil(StencilCommand&& cmd);

void use(GLuint id);

void draw(DrawArrayCommand&& cmd);
void draw(DrawElementsCommand cmd);

bool set_vec3(GLuint id, const std::string_view name, const glm::vec3& val);
bool set_vec4(GLuint id, const std::string_view name, const glm::vec4& val);
bool set_mat4(GLuint id, const std::string_view name, const glm::mat4& val);
bool set_int(GLuint id, const std::string_view name, GLint value);

struct ShaderProgramInterface final {
    GLint uniforms_count = 0;
    std::unordered_map<std::string, GLenum> uniform_block;
    GLint input_count = 0;
    std::unordered_map<std::string, GLenum> input_block;
};
std::ostream& operator << (std::ostream& os, const ShaderProgramInterface& i);

ShaderProgramInterface get_program_interface(GLuint program);

}
