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
    AttribPointer(size_t i, size_t w, size_t s);
};


class Item {
    enum State {
        NONE = 0x0,
        VERTEX_ARRAY_BOUND = 0x1,
        VERTEX_BUFFER_BOUND = 0x2,
        ATTRIBS_SET = 0x4,
        ELEMENT_BUFFER_BOUND = 0x8
    };

public:
    Item(const Coordinates& coords, const AttribPointer& attribs);
    Item(Coordinates&& coords, Coordinates&& normals, AttribPointer&& attribs);
    Item(const Coordinates& coords, const Indices& indices,
         const AttribPointer& attribs);
    ~Item() = default;

    void draw(Program& program);

    const glm::mat4& model() const { return _model; }

private:
    bool has_indices() const;

private:
    Coordinates _coordinates, _normals;
    Indices _indices;
    AttribPointer _attribs;

    VertexArrayBuffer _vao;
    VertexBuffer _vbo, _vbo_2;
    ElementBuffer _ebo;

    State _current_state = NONE;

    glm::mat4 _model;
};

}
