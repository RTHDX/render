#pragma once

#include <vector>
#include <string_view>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


namespace opengl {

void framebuffer_size_callback(GLFWwindow*, int width, int height);

class Object {
public:
    Object() = default;
    virtual ~Object() = default;

    virtual void initialize() = 0;

    GLuint id() const { return _id; }
    GLuint* id_ptr() { return &_id; }

protected:
    void id(GLuint id) { _id = id; }

private:
    GLuint _id {};
};


class Context final : public Object {
public:
    static Context& instance();

    void initialize() override;
    void dump() const;

    void viewport(int width, int height) const;
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

    bool is_bound() const { return _is_bound; }
    void is_bound(bool val) { _is_bound = val; }

private:
    bool _is_bound;
};


class VertexArrayBuffer final : public Buffer {
public:
    VertexArrayBuffer() = default;
    ~VertexArrayBuffer() override;

    void bind();
    void initialize() override;
};


class VertexBuffer final : public Buffer {
public:
    VertexBuffer() = default;
    ~VertexBuffer() override;

    void initialize() override;

    void bind(float* data, size_t len);
    void set_layout(GLuint index, GLuint width, GLsizei stride);
};


class ElementBuffer final : public Buffer {
public:
    ElementBuffer() = default;
    ~ElementBuffer() override;

    void initialize() override;

    void bind(uint32_t* data, size_t len);
};


class Shader final : public Object {
public:
    Shader() = default;
    Shader(GLuint shader_type);
    ~Shader() override;

    void initialize() override;

    void compile(const char* source);
    bool is_compiled() const { return _is_compiled; }

private:
    void is_compiled(bool val) { _is_compiled = val; }
    void check_shader() const;

private:
    bool _is_compiled;
    GLuint _shader_type;
};


class Program final : public Object {
public:
    Program() = default;
    ~Program() override;

    void initialize() override;

    void attach_shader(const Shader& shader);
    void link_program();
    void use();

private:
    void check_program() const;
};


class Pipeline {
public:
    Pipeline() = default;
    ~Pipeline() = default;

    Pipeline(const Pipeline&) = delete;
    Pipeline& operator = (const Pipeline&) = delete;
    Pipeline(Pipeline&&) = delete;
    Pipeline& operator = (Pipeline&&) = delete;

    Pipeline& create_vertex_shader(const std::string_view src);
    Pipeline& create_fragment_shader(const std::string_view src);
    Pipeline& create_program();
    Pipeline& create_vao();
    Pipeline& create_vbo(float* data, size_t len);
    Pipeline& create_ebo(uint32_t* data, size_t len);
    Pipeline& set_layot(GLuint index, GLuint width, GLuint stride);

    void draw();

private:
    Shader _vrt_shader,
           _frg_shader;
    Program _program;
    VertexArrayBuffer _vertex_array_buffer;
    VertexBuffer _vertex_buffer_object;
    ElementBuffer _element_buffer;

    size_t _vertex_count = 0;
};

}
