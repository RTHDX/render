#include <cassert>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include "item.hpp"


namespace opengl {

template <typename T>
static void bind_vbo(GLuint id, const std::vector<T>& input) {
    SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));
    SAFE_CALL(glBufferData(GL_ARRAY_BUFFER,
                           input.size() * sizeof(T),
                           input.data(),
                           GL_STATIC_DRAW));
}

struct _VertAttribData {
    GLuint index;
    GLuint stride;
    size_t width;
    void* offset;

public:
    _VertAttribData(GLuint i, GLuint s, size_t w, void* o)
        : index(i), stride(s), width(w), offset(o)
    {}
};

static void set_vertex_attribs(_VertAttribData&& data) {
    SAFE_CALL(glEnableVertexAttribArray(data.index));
    SAFE_CALL(glVertexAttribPointer(data.index,
                                    data.stride,
                                    GL_FLOAT,
                                    GL_FALSE,
                                    data.width,
                                    data.offset));
}

void BItem::initialize() {
    set_up_vertex_array();
}

void BItem::set_up_vertex_array() {
    glGenVertexArrays(1, &_vao);
    update_state(State::VERTEX_ARRAY_GENERATED);
    bind_vao();
}

void BItem::bind_vao() {
    SAFE_CALL(glBindVertexArray(_vao));
}

void BItem::unbind_vao() {
    SAFE_CALL(glBindVertexArray(0));
}

BItem::~BItem() {
    SAFE_CALL(glDeleteVertexArrays(1, &_vao));
}


Item::Item(std::vector<VertexData>&& vertices)
    : _vertices(std::move(vertices))
    , _model(glm::mat4(1.0))
{}

void Item::initialize() {
    set_up_vertex_array();
    set_up_vertex_buffer();
}

void Item::set_up_vertex_array() {
    SAFE_CALL(glGenVertexArrays(1, &_vao));
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
    SAFE_CALL(glGenBuffers(1, &_vbo_coordinates));
    update_state(State::VERTEX_POS_GENERATED);
    SAFE_CALL(glGenBuffers(1, &_vbo_normals));
    update_state(State::VERTEX_NORMAL_GENERATED);

    bind_vertex_array();
    bind_vbo<VertexData>(_vbo_coordinates, _vertices);
    update_state(State::VERTEX_POS_BOUND);
    bind_vbo<VertexData>(_vbo_normals, _vertices);
    update_state(State::VERTEX_NORMAL_BOUND);

    set_vertex_attribs(_VertAttribData(0, 3, sizeof (VertexData),
                       (void*)(offsetof(VertexData, position))));
    update_state(State::VERTEX_POS_ATTRIB_SET);
    set_vertex_attribs(_VertAttribData(1, 3, sizeof (VertexData),
                       (void*)(offsetof(VertexData, normal))));
    update_state(State::VERTEX_NORMAL_ATTRIB_SET);
    unbind_vertex_array();
}

void Item::draw() {
    bind_vertex_array();
    glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
    unbind_vertex_array();
}


TItem::TItem(std::vector<VertexTexData>&& vertices)
    : _vertices(std::move(vertices))
{}

void TItem::initialize() {
    BItem::initialize();

    set_up_vertex_buffers();
}

void TItem::set_up_vertex_buffers() {
    SAFE_CALL(glGenBuffers(1, &_vbo_coordinates));
    SAFE_CALL(glGenBuffers(1, &_vbo_normals));
    SAFE_CALL(glGenBuffers(1, &_tex_coordinates));

    bind_vao();
    bind_vbo<VertexTexData>(_vbo_coordinates, _vertices);
    bind_vbo<VertexTexData>(_vbo_normals, _vertices);
    bind_vbo<VertexTexData>(_tex_coordinates, _vertices);

    set_vertex_attribs(_VertAttribData(0, 3, sizeof(VertexTexData),
                       (void*)(offsetof(VertexTexData, position))));
    set_vertex_attribs(_VertAttribData(1, 3, sizeof(VertexTexData),
                       (void*)(offsetof(VertexTexData, normal))));
    set_vertex_attribs(_VertAttribData(2, 2, sizeof (VertexTexData),
                       (void*)(offsetof(VertexTexData, tex_pos))));

    unbind_vao();
}

void TItem::draw() {
    bind_vao();
    glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
    unbind_vao();
}

}
