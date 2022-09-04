#include <OpenGL/opengl_proc.hpp>

#include "Item.hpp"


void Item_2D::update() {
    auto& animation = active_animation();
    if (animation.is_valid()) { animation.update(); }
}

void Item_2D::render(GLuint program) {
    assert(opengl::Context::instance().active_program() == program);
    auto& animation = active_animation();
    if (animation.is_valid()) {
        opengl::set_int(program, "current_frame", animation.current_index());
        opengl::set_int(program, "total_frames", animation.total_frames());
    }
    opengl::set_mat4(program, "model", model);
    opengl::draw(std::move(opengl::DrawElementsCommand(
        .vao = vao,
        .count = indices.size()
    )));
}

render::Animation& Item_2D::active_animation() const {
    static render::Animation default_anim;
    for (const render::Animation& animation : animations) {
        if (animation.is_active()) { return animation; }
    }
    return default_anim;
}
