#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "opengl.hpp"


namespace opengl {

using Index = uint32_t;
using Indices = std::vector<Index>;
using Coordinates = std::vector<float>;

struct AttribPointer {
    size_t index;
    size_t width;
    size_t stride;

public:
    AttribPointer() = default;
    AttribPointer(size_t i, size_t w, size_t s);
};


struct VertexData {
    glm::vec3 position;
    glm::vec3 normal;
};


class Item {
    enum State {
        NONE                 = 0x0,
        VERTEX_ARRAY_SET     = 0x1,
        VERTEX_BUFFER_SET    = 0x2,
        ELEMNT_BUFFER_SET    = 0x4,
        ATTRIBS_SET          = 0x8,
        ELEMENT_BUFFER_BOUND = 0x10
    };

public:
    Item() = default;
    Item(std::vector<VertexData>&& vertices);
    ~Item() = default;

    void draw(Program& program);

    const glm::mat4& model() const { return _model; }
    const std::vector<VertexData>& vertices() const { return _vertices; }

private:
    void set_up_vertex_array();
    void set_up_vertex_buffer();
    void set_up_element_buffer();

private:
    std::vector<VertexData> _vertices;
    glm::mat4 _model;

    GLuint _vao = 0, _vbo_coordinates = 0, _vbo_normals = 0;

    State _current_state = NONE;
};

}
