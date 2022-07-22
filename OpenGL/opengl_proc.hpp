#pragma once

#include <vector>
#include <filesystem>
#include <string_view>
#include <string>

#include <glm/glm.hpp>
#include <glad/glad.h>


namespace opengl {

class Context {
public:
    static Context& instance();

    void initialize(bool = false);
    void dump() const;

    void viewport(int width, int height) const;
    glm::vec4& background() { return _background; }
    void background(const glm::vec4& color);

    void draw_background() const;

    GLint active_program() const;
    GLint bound_vao() const;

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

bool check_shader(GLuint id);
bool check_program(GLuint id);


std::vector<GLuint> gen_vertex_array(size_t count);
GLuint gen_vertex_array();
void free_vertex_array(const std::vector<GLuint>& in);
void free_vertex_array(GLuint id);

std::vector<GLuint> gen_vertex_buffers(size_t count);
GLuint gen_vertex_buffers();
void free_vertex_buffers(const std::vector<GLuint>& in);
void free_vertex_buffer(GLuint id);

template <typename T>
inline void bind_vbo(GLuint id, const std::vector<T>& in) {
    assert(Context::instance().bound_vao() > 0);
    const T* data = in.data();
    const size_t width = in.size() * sizeof (T);
    SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));
    SAFE_CALL(glBufferData(GL_ARRAY_BUFFER, width, data, GL_STATIC_DRAW));
}

template <typename T> struct VertexAttribCommand {
    GLuint index;
    GLuint stride;
    void* offset;

    size_t width = sizeof(T);
};

void bind_vao(GLuint id);

template<typename T>
inline void set_vertex_attrib(const VertexAttribCommand<T>& cmd) {
    SAFE_CALL(glEnableVertexAttribArray(cmd.index));
    SAFE_CALL(glVertexAttribPointer(cmd.index, cmd.stride, GL_FLOAT, GL_FALSE,
                                    cmd.width, cmd.offset));
}

template<typename T>
inline void do_vertex_attrib_cmds(std::vector<VertexAttribCommand<T>>&& cmds) {
    assert(Context::instance().bound_vao() > 0);

    for (const auto& command : cmds) {
        set_vertex_attrib<T>(command);
    }
}

void use(GLuint id);

struct DrawArrayCommand {
    GLuint vao;
    size_t count;
    size_t first = 0;
    GLenum mode = GL_TRIANGLES;
};


void draw(DrawArrayCommand&& cmd);


bool set_vec3(GLuint id, const std::string_view name, const glm::vec3& val);
bool set_vec4(GLuint id, const std::string_view name, const glm::vec4& val);
bool set_mat4(GLuint id, const std::string_view name, const glm::mat4& val);

}
