#include <cassert>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include "item.hpp"


namespace opengl {

static glm::mat4 rotate_mat() {
    float rot = glm::radians(1.0f);
    return glm::mat4{
        cos(rot),  0.0, sin(rot), 0.0,
        0.0,       1.0, 0.0,      0.0,
        -sin(rot), 0.0, cos(rot), 0.0,
        0.0,       0.0, 0.0,      1.0
    };
}

Item::Item(std::vector<VertexData>&& vertices)
    : _vertices(std::move(vertices))
    //, _model(glm::mat4(1.0))
    , _model(rotate_mat())
{}

void Item::initialize() {
    set_up_vertex_array();
    set_up_vertex_buffer();
}

void Item::set_up_vertex_array() {
    glGenVertexArrays(1, &_vao);
    update_state(State::VERTEX_ARRAY_GENERATED);
    bind_vertex_array();
}

void Item::bind_vertex_array() {
    glBindVertexArray(_vao);
}

void Item::unbind_vertex_array() {
    glBindVertexArray(0);
}

void Item::set_up_vertex_buffer() {
    glGenBuffers(1, &_vbo_coordinates);
    update_state(State::VERTEX_POS_GENERATED);
    glGenBuffers(1, &_vbo_normals);
    update_state(State::VERTEX_NORMAL_GENERATED);

    bind_vertex_array();
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_coordinates);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(VertexData),
                 _vertices.data(),
                 GL_STATIC_DRAW);
    update_state(State::VERTEX_POS_BOUND);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof (VertexData),
                 _vertices.data(),
                 GL_STATIC_DRAW);
    update_state(State::VERTEX_NORMAL_BOUND);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof (VertexData),
                          (void*)(offsetof(VertexData, position)));
    update_state(State::VERTEX_POS_ATTRIB_SET);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof (VertexData),
                          (void*)(offsetof(VertexData, normal)));
    update_state(State::VERTEX_NORMAL_ATTRIB_SET);
    unbind_vertex_array();
}

void Item::draw() {
    _model = _model * rotate_mat();
    bind_vertex_array();
    glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
    unbind_vertex_array();
}

}
