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
using stencil_idx_t = GLubyte;


class Context {
public:
    static Context& instance();

    void initialize(bool = false);
    void initialize_light(bool = false);
    void dump() const;

    void viewport(int width, int height) const;
    glm::vec4& background() { return _background; }
    void background(const glm::vec4& color);
    void background(glm::u8vec4 color);

    bool is_initialized() const { return initialized_; }
    bool is_context_active() const;

    void draw_background() const;

    GLint active_program() const;
    GLint bound_vao() const;
    GLint bound_texture_2d() const;
    GLint bound_texture_2d_array() const;
    GLint bound_framebuffer() const;

private:
    Context() = default;

private:
    glm::vec4 _background;
    bool initialized_ = false;
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

struct TextureData final {
    GLuint id = 0;
    GLenum target; // GL_TEXTURE_1D, GL_TEXTURE_2D ...
    GLint w, h;
    GLint format; // GL_DEPTH_COMPONENT GL_DEPTH_STENCIL GL_RED GL_RG GL_RGB GL_RGBA
    GLenum type; // GL_UNSIGNED_BYTE...
    GLenum wrap_s, wrap_t, min_filter, mag_filter;

    void free();
    bool is_valid() const;
};
std::ostream& operator << (std::ostream& os, const TextureData& tex);


struct TextureDataArray2D final {
    TextureData tex_data;
    size_t tile_count_w {0}, tile_count_h {0};

    GLsizei tile_w() const; // pixels
    GLsizei tile_h() const; // pixels
    GLsizei total_tiles() const;
    bool is_valid() const;
    GLenum internal_format() const;
    GLsizei tile_offset(int x, int y) const;

    void dump_meta() const;
    void free();
};


struct RenderData final {
    GLuint program;
    GLuint vao;
    std::vector<GLuint> vertex_buffers;
    GLuint ebo;
    stencil_idx_t stencil_ref;
    size_t ebo_count;

    void free();
};


struct InstantRenderData final {
    RenderData render_data;
    std::unordered_map<std::string, GLuint> buffers;
    GLsizei instance_count;

    void free();
};


struct FramebufferData final {
    GLuint fbo;
    GLenum attachment_point; // GL_COLOR_ATTACHMENT0 ... GL_COLOR_ATTACHMENT31
    TextureData texture;
    GLenum target {GL_FRAMEBUFFER};

    GLenum status() const;
};
std::ostream& operator << (std::ostream& os, const FramebufferData& f);

void attach_texture(const FramebufferData& fbuff, const TextureData& tex);

GLuint gen_texture(GLenum target = GL_TEXTURE_2D);
void bind_texture(const glm::ivec2& dims,
                  std::vector<glm::vec3>&& texture);
void bind_texture(const glm::ivec2& dims,
                  byte_t* texture);
void bind_texture(GLenum target, GLuint id);
void set_texture_meta(byte_t* raw_data, const TextureData& params);
void set_texture_2d_array_meta(byte_t* raw_data, const TextureDataArray2D&);
void activate_texture(const TextureActivationCommand& cmd);
void free_texture(GLuint id);

void apply_stencil(StencilCommand&& cmd);

void use(GLuint id);

void draw(const DrawArrayCommand& cmd);
void draw(const DrawElementsCommand& cmd);
void draw_array_framebuffer(const DrawArrayFramebuffer& cmd);
void draw_instance_array(const DrawArrayInstanced& cmd);
void draw_instance_elements(const DrawElementInstanced& cmd);

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


struct buffer_bind_guard final {
    buffer_bind_guard() = delete;
    buffer_bind_guard(const buffer_bind_guard&) = delete;
    buffer_bind_guard& operator = (const buffer_bind_guard&) = delete;
    buffer_bind_guard(buffer_bind_guard&&) = delete;
    buffer_bind_guard& operator = (buffer_bind_guard&&) = delete;

    buffer_bind_guard(VertexArrayBindCommand&& cmd);
    buffer_bind_guard(BufferBindCommand&& cmd);
    ~buffer_bind_guard();

private:
    enum __buffer_bind_mode__ {
        VAO_BINDER,
        VBO_BINDER
    } mode_;
    GLenum buff_type_ = 0;
};

}
