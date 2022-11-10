#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Loader/opengl_converter.hpp>


inline glm::mat4 scale_matrix(const glm::vec3& scale) {
    return {
        scale.x, 0.0, 0.0, 0.0,
        0.0, scale.y, 0.0, 0.0,
        0.0, 0.0, scale.z, 0.0,
        0.0, 0.0, 0.0,     1.0
    };
}

inline glm::mat4 move_matrix(const glm::vec3& move) {
    return {
        1.0, 0.0, 0.0, move.x,
        0.0, 1.0, 0.0, move.y,
        0.0, 1.0, 1.0, move.z,
        0.0, 0.0, 0.0, 1.0
    };
}


class Item3D {
public:
    Item3D() = default;
    ~Item3D();

    void open(const std::string& path);
    void draw() const;
    void modify(glm::mat4&& modificator);

    const glm::mat4& model() const { return model; }

private:
    glm::mat4 _model {1.0};
    GLuint _vao {0};
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
