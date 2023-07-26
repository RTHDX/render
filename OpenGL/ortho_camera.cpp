#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "camera.hpp"

namespace opengl {

OrthoCamera::OrthoCamera(const glm::vec3& pos, float w, float h, float factor)
    : pos_(pos)
    , clip_space_{.factor = factor, .width = w, .height = h}
{
    update_projection();
    update_view();
    update_viewport(w, h);
}

const glm::mat4& OrthoCamera::projection() const {
    return projection_;
}

const glm::mat4& OrthoCamera::view() const {
    return view_;
}

const glm::mat4& OrthoCamera::ipv() const {
    return ipv_;
}

const glm::vec4& OrthoCamera::viewport() const {
    return viewport_;
}

void OrthoCamera::move(Direction dir) {
    glm::vec3 dir_vec = {0.0, 0.0, 0.0};
    switch (dir) {
    case Direction::FORWARD:
        dir_vec = NORTH;
        break;
    case Direction::BACKWARD:
        dir_vec = SOUTH;
        break;
    case Direction::LEFT:
        dir_vec = WEST;
        break;
    case Direction::RIGHT:
        dir_vec = EAST;
        break;
    case Direction::FORWARD_LEFT:
        dir_vec = EAST_NORTH;
        break;
    case Direction::FORWARD_RIGHT:
        dir_vec = WEST_NORTH;
        break;
    case Direction::BACKWARD_LEFT:
        dir_vec = EAST_SOUTH;
        break;
    case Direction::BACKWARD_RIGHT:
        dir_vec = WEST_SOUTH;
        break;
    case Direction::UP:
        dir_vec = UP_MOVE;
        break;
    case Direction::DOWN:
        dir_vec = DOWN_MOVE;
        break;
    }
    auto move_mat = glm::translate(glm::mat4(1.0), dir_vec);
    pos_ = move_mat * glm::vec4(pos_, 1.0);
    update_view();
}

void OrthoCamera::zoom_in() {
    clip_space_.factor -= step_;
    move(Direction::DOWN);
    update_projection();
}

void OrthoCamera::zoom_out() {
    clip_space_.factor += step_;
    move(Direction::UP);
    update_projection();
}

void OrthoCamera::zoom_step(float step) { step_ = step; }
float OrthoCamera::zoom_step() const { return step_; }

void OrthoCamera::update_sizes(size_t w, size_t h) {
    clip_space_.width = float(w);
    clip_space_.height = float(h);
    update_viewport(w, h);
    update_projection();
}

glm::vec3 OrthoCamera::screen_to_world(const glm::ivec2& screen) {
    glm::vec3 temp = glm::vec3(screen, 0.0f);
    // In OpenGL the origin is at the bottom left corner.
    temp.y = viewport_.w - temp.y;
    // Normalize between 0 and 1.
    temp = temp / glm::vec3(viewport_.z, viewport_.w, 1.0);
    // Remap coordinates to range -1 to 1.
    temp = temp * 2.0f - 1.0f;

    glm::vec4 out = ipv() * glm::vec4(temp, 1.0f);
    return {out.x * 0.5, 0.0f, out.z * 0.5};
}

glm::vec2 OrthoCamera::world_pixel_size() const {
    return {
        2 * clip_space_.half_width() / viewport_.z,
        2 * clip_space_.half_height() / viewport_.w
    };
}

float OrthoCamera::clip_space_factor() const {
    return clip_space_.factor;
}

void OrthoCamera::update_viewport(size_t w, size_t h) {
    viewport_.z = float(w);
    viewport_.w = float(h);
}

void OrthoCamera::update_projection() {
    projection_ = glm::ortho(
        clip_space_.half_width(),  -clip_space_.half_width(),
        clip_space_.half_height(), -clip_space_.half_height(),
        Z_NEAR, Z_FAR
    );
    update_ipv();
}

void OrthoCamera::update_view() {
    view_ = glm::lookAt(pos_, pos_ + glm::vec3(0.0, -1.0, 0.0), UP);
    update_ipv();
}

void OrthoCamera::update_ipv() {
    ipv_ = glm::inverse(projection_ * view_);
}

}
