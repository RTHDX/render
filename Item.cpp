#include "Item.hpp"

namespace render {

Item_2D::Item_2D(glm::mat4&& model, Animation&& animation, vertex_t&& box,
                 const opengl::TextureArray& atlas)
    : _model(std::move(model))
    , _animation(std::move(animation))
    , _box(std::move(box))
    , _atlas(atlas)
{}

Item_2D::~Item_2D() {
    opengl::free_vertex_buffer(_vbo);
    opengl::free_element_buffer(_ebo);
    opengl::free_vertex_array(_vao);
}

void Item_2D::init() {
    _vao = opengl::gen_vertex_array();
    _ebo = opengl::gen_element_buffer();
    _vbo = vertex_t::gen_buffers(_vao, _box);
    opengl::bind_vao(_vao);
    opengl::bind_ebo(_ebo, _indices);
    if (!_atlas.is_read) {
        _atlas.read();
    }
}

void Item_2D::update() {
    _animation.update();
}

void Item_2D::render(GLuint program);

}
