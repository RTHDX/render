#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Loader/opengl_converter.hpp>
#include <OpenGL/camera.hpp>
#include <OpenGL/light.hpp>


class Item3D {
public:
    Item3D() = default;
    Item3D(const std::filesystem::path& vertex,
           const std::filesystem::path& fragment,
           const glm::vec4& color);
    ~Item3D();

    void open(const std::string& path);
    void draw() const;
    void modify(glm::mat4&& modificator);
    void finalyze();

    const glm::mat4& model() const { return _model; }
    GLuint program() const { return _program; }
    void color(const glm::vec4& color) { _color = color; }
    const glm::vec4& color() const { return _color; }

private:
    glm::mat4 _model {1.0};
    GLuint _vao {0};
    GLuint _program{ 0 };
    glm::vec4 _color{0.0, 0.0, 0.0, 1.0};

    std::vector<opengl::buffers_t> _vertex_input;
    std::vector<loader::Vertices> _vertices;
};

struct ShaderUniformData {
    glm::vec4 color;
    glm::vec3 light_position;
    glm::vec4 light_color;
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 model;
};

void pass_shader_uniforms(GLuint program, ShaderUniformData&& data,
                          const Item3D& item);

class Scene {
public:
    Scene() = default;
    Scene(std::vector<Item3D>&& items, opengl::Light&& light,
          opengl::Camera&& camera);
    ~Scene();

    void draw();

private:
    std::vector<Item3D> _items;
    opengl::Light _light;
    opengl::Camera _camera;
};
