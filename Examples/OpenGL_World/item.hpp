#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Loader/opengl_converter.hpp>
#include <OpenGL/camera.hpp>
#include <OpenGL/light.hpp>


struct ItemInputData {
    std::filesystem::path vertex;
    std::filesystem::path fragment;
    std::filesystem::path vertex_selection;
    std::filesystem::path fragment_selection;

    glm::vec4 color;
    glm::vec4 selection_color;

    bool is_selectable = true;
};


class Item3D {
public:
    static constexpr glm::vec3 SELECTION_SCALE = {1.1, 1.1, 1.1};

    Item3D() = default;
    Item3D(const std::filesystem::path& vertex,
           const std::filesystem::path& fragment,
           const glm::vec4& color,
           bool is_selectable = true);
    explicit Item3D(ItemInputData&& data);
    ~Item3D();

    void open(const std::string& path);
    void draw() const;
    void modify(glm::mat4&& modificator);
    void finalyze();

    const glm::mat4& model() const { return _model; }
    GLuint program() const { return _program; }

    void color(const glm::vec4& color) { _color = color; }
    const glm::vec4& color() const {
        return is_active() ? _selection_color : _color;
    }

    GLuint selection_program() const { return _selection_program; }
    const glm::vec4& selection_color() const { return _selection_color; }

    void id(int id) { if (_id == -1) { _id = id; } }
    int id() const { return _id; }

    bool is_valid() const { return _vao != 0 && _program != 0; }
    bool is_active() const { return _is_active; }

    bool activate(int);
    bool activate();
    void deactivate();

private:
    glm::mat4 _model {1.0};
    GLuint _vao {0};
    GLuint _program {0};
    glm::vec4 _color {0.0, 0.0, 0.0, 0.5};

    GLuint _selection_program {0};
    glm::vec4 _selection_color {0.0, 0.0, 0.0, 1.0};

    int _id {-1};
    bool _is_active {false};
    bool _is_selectable;

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

struct SelectionShaderUniformData {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
};

void pass_selection_shader_uniforms(
    GLuint program,
    SelectionShaderUniformData&& data,
    const Item3D& item
);

class Scene {
public:
    Scene() = default;
    Scene(std::vector<Item3D>&& items, opengl::Light&& light,
          opengl::Camera&& camera);
    Scene(opengl::Light&& light, opengl::Camera&& camera);
    ~Scene();

    void draw();

    std::vector<Item3D>& items() { return _items; }
    opengl::Camera& camera() { return _camera; }
    bool activate_index(GLuint index);

    void append(Item3D&& item);
    void clear();

    int width() const  { return _camera.width(); }
    int height() const { return _camera.height(); }

private:
    std::vector<Item3D> _items;
    opengl::Light _light;
    opengl::Camera _camera;
};
