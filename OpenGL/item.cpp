#include <cassert>
#include <glad/glad.h>

#include "item.hpp"


namespace opengl {

Item::Item(const Coordinates& coords, const AttribPointer& attribs)
    : _coordinates(coords)
    , _attribs(attribs)
{
    _vao.initialize();
    _vbo.initialize();

    _vao.bind();
    _vbo.bind(_coordinates.data(), _coordinates.size() * sizeof (float));
    _vbo.set_layout(attribs.index, attribs.width, attribs.stride);
    _current_state = ATTRIBS_SET;
}

Item::Item(const Coordinates& coords, const Indices& indices,
           const AttribPointer& attribs)
    : Item(coords, attribs)
{
    _indices = indices;
    _ebo.initialize();
    _ebo.bind(_indices.data(), _indices.size() * sizeof (Index));
    _current_state = ELEMENT_BUFFER_BOUND;
}

void Item::draw(Program& program) {
    program.use();
    if (has_indices()) {
        assert(_current_state == ELEMENT_BUFFER_BOUND);
        glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
    } else {
        assert(_current_state == ATTRIBS_SET);
        _vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, _coordinates.size() / _attribs.stride);
    }
}

bool Item::has_indices() const { return _indices.size() > 0; }

}
