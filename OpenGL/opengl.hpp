#pragma once

#include <vector>
#include <string_view>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


namespace opengl {

void framebuffer_size_callback(GLFWwindow*, int width, int height);

class Object {
public:
    Object() = default;
    virtual ~Object() = default;

    //Object(const Object&) = delete;
    //Object& operator = (const Object&) = delete;
    //Object(Object&&) = default;
    //Object& operator = (Object&&) = default;

    virtual void initialize() = 0;

    GLuint id() const { return _id; }
    GLuint* id_ptr() { return &_id; }

protected:
    void id(GLuint id) { _id = id; }

private:
    GLuint _id {};
};


class Context {
public:
    static Context& instance();

    void initialize(bool=false);
    void dump() const;

    void viewport(int width, int height) const;
    glm::vec4& background() { return _background; }
    void background(const glm::vec4& color);

    void draw_background() const;

private:
    Context() = default;

private:
    glm::vec4 _background;
};


class Buffer : public Object {
public:
    Buffer();
    ~Buffer() override = default;

    bool is_bound()   const { return _is_bound; }
    void is_bound(bool val) { _is_bound = val; }

private:
    bool _is_bound;
};


class VertexArrayBuffer final : public Buffer {
public:
    VertexArrayBuffer();
    ~VertexArrayBuffer() override;

    void bind();
    void unbind();
    void initialize() override;
};


class VertexBuffer final : public Buffer {
public:
    VertexBuffer() = default;
    ~VertexBuffer() override;

    void initialize() override;

    void bind(const float* data, size_t len);
    void set_layout(GLuint index, GLuint width, GLsizei stride);
};


class ElementBuffer final : public Buffer {
public:
    ElementBuffer() = default;
    ~ElementBuffer() override;

    void initialize() override;

    void bind(const uint32_t* data, size_t len);
};


class Shader final : public Object {
public:
    Shader() = default;
    Shader(GLuint shader_type);
    ~Shader() override;

    void initialize() override;

    void compile(const std::string_view source);
    bool is_compiled() const { return _is_compiled; }

    GLuint shader_type() const { return _shader_type; }

private:
    void is_compiled(bool val) { _is_compiled = val; }
    void check_shader() const;

private:
    bool _is_compiled;
    GLuint _shader_type;
};


class Program final : public Object {
public:
    enum State {
        NONE                     = 0x0,
        INITIALIZED              = 0x1,
        VERTEX_SHADER_CREATED    = 0x2,
        FRAGEMENT_SHADER_CREATED = 0x4,
        LINKED                   = 0x8,
        ACTIVE                   = 0x10
    };

public:
    Program(const std::string_view label="UNKNOWN");
    ~Program() override;

    void initialize() override;

    void attach_shader(const Shader& shader);
    void attach_shader(GLuint shader_type, const std::string_view src);
    void link_program();
    void use();

    const Shader& vertex_shader() const { return _vertex_shader; }
    const Shader& fragment_shader() const { return _fragment_shader; }

    void set_vec4(const std::string_view name, const glm::vec4& val) const;
    void set_vec3(const std::string_view name, const glm::vec3& val) const;
    void set_mat4(const std::string_view name, const glm::mat4& value) const;

private:
    void check_program() const;
    void update_state(State new_state);

private:
    std::string _label;

    Shader _vertex_shader, _fragment_shader;
    State _current_state;
};


struct VertexBufferAttrib {
    GLuint index;
    GLuint stride;
    size_t width;
    size_t total_width;
    float* data;
    void* offset;
};

void on_gl_error(GLenum error_code, const char* call, const char* file,
                 int line);
#define SAFE_CALL(gl_call)\
    gl_call;\
    opengl::on_gl_error(glGetError(), #gl_call, __FILE__, __LINE__);
//#define SAFE_CALL(call) call


GLuint create_program(const std::filesystem::path& vertex_path,
                      const std::filesystem::path& fragment_path);
GLuint create_program(const std::string& vertex_shader,
                      const std::string& fragment_shader);
GLuint create_vao();
void bind_vao(GLuint);
GLuint create_vbo(GLuint vao, VertexBufferAttrib attribs);

bool set_vec3(GLuint id, const std::string_view name, const glm::vec3& val);
bool set_vec4(GLuint id, const std::string_view name, const glm::vec4& val);
bool set_mat4(GLuint id, const std::string_view name, const glm::mat4& val);

bool check_shader(GLuint id);
bool check_program(GLuint id);

}
