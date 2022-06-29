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
        NONE                 = 0x0,
        VERTEX_ARRAY_SET     = 0x1,
        VERTEX_BUFFER_SET    = 0x2,
        ELEMNT_BUFFER_SET    = 0x4,
        ATTRIBS_SET          = 0x8,
        ELEMENT_BUFFER_BOUND = 0x10
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

    void set_up_vertex_array();
    void set_up_vertex_buffer();
    void set_up_element_buffer();

private:
    Coordinates _coordinates, _normals;
    Indices _indices;
    AttribPointer _attribs;
    GLuint _vao, _vbo_coordinates, _vbo_normals;

    State _current_state = NONE;

    glm::mat4 _model;
};

}
