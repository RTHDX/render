#include <cassert>
#include <glad/glad.h>

#include "item.hpp"


namespace opengl {


AttribPointer::AttribPointer(size_t i, size_t w, size_t s)
    : index(i)
    , width(w)
    , stride(s)
{}


Item::Item(const Coordinates& coords, const AttribPointer& attribs)
    : _coordinates(coords)
    , _attribs(attribs)
    , _model(glm::mat4(1.0f))
{}

Item::Item(Coordinates&& coords, Coordinates&& normals, AttribPointer&& attrs)
    : _coordinates(std::move(coords))
    , _normals(std::move(normals))
    , _attribs(std::move(attrs))
    , _model(glm::mat4(1.0f))
{
}

Item::Item(const Coordinates& coords, const Indices& indices,
           const AttribPointer& attribs)
    : Item(coords, attribs)
{
}

void Item::draw(Program& program) {
    program.use();
    if (has_indices()) {
        assert(_current_state == ELEMENT_BUFFER_BOUND);
        glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
    } else {
        assert(_current_state == ATTRIBS_SET);
        //_vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, _coordinates.size() / _attribs.stride);
    }
}

bool Item::has_indices() const { return _indices.size() > 0; }

}
