#include <cassert>
#include <glad/glad.h>

#include "item.hpp"


namespace opengl {


AttribPointer::AttribPointer(size_t i, size_t w, size_t s)
    : index(i)
    , width(w)
    , stride(s)
{}

Item::Item(std::vector<VertexData>&& vertices)
    : _vertices(std::move(vertices))
    , _model(glm::mat4(1.0))
{}

void Item::draw(Program& program) {
    program.use();
    //if (has_indices()) {
    //    assert(_current_state == ELEMENT_BUFFER_BOUND);
    //    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
    //} else {
    //    assert(_current_state == ATTRIBS_SET);
    //    _vao.bind();
    //    glDrawArrays(GL_TRIANGLES, 0, _coordinates.size() / _attribs.stride);
    //}
}

//bool Item::has_indices() const { return _indices.size() > 0; }

}
