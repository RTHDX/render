#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"


namespace opengl {


template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;


Camera Camera::create_perspective(float width,
                                    float height,
                                    float field_of_view,
                                    const glm::vec3& pos,
                                    const glm::vec3& look_at) {
    perspective_clip_t clip;
    clip.width   = width;
    clip.height  = height;
    clip.fov     = field_of_view;
    clip.look_at = look_at;
    Camera cam;
    cam.clip_ = std::move(clip);
    cam.pos_  = pos;
    return cam;
}

Camera Camera::create_topdown(float width,
                              float height,
                              float factor,
                              const glm::vec3& pos) {
    ortho_clip_t clip;
    clip.width   = width;
    clip.height  = height;
    clip.factor  = factor;
    Camera cam;
    cam.clip_ = std::move(clip);
    cam.pos_  = pos;
    return cam;
}

Camera Camera::create_topdown(float width,
                              float height,
                              const glm::vec3& pos) {
    return Camera::create_topdown(
        width,
        height,
        pos.y / ortho_clip_t::HEIGHT_FACTOR,
        pos
    );
}

glm::mat4 Camera::projection() const {
    switch (mode()) {
    case Mode::PERSPECTIVE:
        return std::get<perspective_clip_t>(clip_).projection();
    case Mode::TOP_DOWN:
        return std::get<ortho_clip_t>(clip_).projection();
    default: return glm::mat4(1.0);
    }
}

glm::mat4 Camera::view() const {
    switch (mode()) {
    case Mode::PERSPECTIVE:
        return std::get<perspective_clip_t>(clip_).view(pos_);
    case Mode::TOP_DOWN:
        return std::get<ortho_clip_t>(clip_).view(pos_);
    default: return glm::mat4(1.0);
    }
}

glm::mat4 Camera::ipv() const {
    return glm::inverse(projection() * view());
}

glm::ivec4 Camera::viewport() const {
    switch (mode()) {
    case Mode::PERSPECTIVE: {
        const auto& clip = std::get<perspective_clip_t>(clip_);
        return {0, 0, clip.width, clip.height};
    } case Mode::TOP_DOWN: {
        const auto& clip = std::get<ortho_clip_t>(clip_);
        return {0, 0, clip.width, clip.height};
    } default: return {0, 0, 1, 1};
    }
}

const glm::vec3& Camera::position() const {
    return pos_;
}

void Camera::position(const glm::vec3& pos) {
    pos_ = pos;
}

void Camera::update_viewport(const glm::vec2& viewport) {
    std::visit(overloaded {
        [viewport](ortho_clip_t& clip) {
            clip.width = viewport.x;
            clip.height = viewport.y;
        },
        [viewport](perspective_clip_t& clip) {
            clip.width = viewport.x;
            clip.height = viewport.y;
        }
    }, clip_);
}

void Camera::zoom_in(float zoom_step) {
    std::visit(overloaded {
        [this, zoom_step](ortho_clip_t& clip) mutable {
            clip.factor  -= zoom_step / ortho_clip_t::HEIGHT_FACTOR;
            this->pos_.y -= zoom_step;
        },
        [](perspective_clip_t& clip) {
            std::cout << "Zooming behavior implemented only for TOP_DOWN\n";
        }
    }, clip_);
}

void Camera::zoom_out(float zoom_step) {
    std::visit(overloaded {
        [this, zoom_step](ortho_clip_t& clip) mutable {
            clip.factor  += zoom_step / ortho_clip_t::HEIGHT_FACTOR;
            this->pos_.y += zoom_step;
        },
        [](perspective_clip_t& clip) {
            std::cout << "Zooming behavior implemented only for TOP_DOWN\n";
        }
    }, clip_);
}

Camera::Mode Camera::mode() const {
    if (std::holds_alternative<ortho_clip_t>(clip_)) {
        return Mode::TOP_DOWN;
    }
    if (std::holds_alternative<perspective_clip_t>(clip_)) {
        return Mode::PERSPECTIVE;
    }

    return Mode::NONE;
}

float Camera::ortho_clip_t::half_width() const {
    return (width * factor) / 2.0;
}

float Camera::ortho_clip_t::half_height() const {
    return (height * factor) / 2.0;
}

glm::mat4 Camera::ortho_clip_t::projection() const {
    return glm::ortho(
        half_width(),  -half_width(),
        half_height(), -half_height(),
        Z_NEAR, Z_FAR
    );
}

glm::mat4 Camera::ortho_clip_t::view(const glm::vec3& eye) const {
    return glm::lookAt(eye, look_at(eye), UP);
}

glm::vec3 Camera::ortho_clip_t::look_at(const glm::vec3& eye) const {
    return {eye.x, 0.0, eye.z};
}

float Camera::perspective_clip_t::aspect_ratio() const {
    return width / height;
}

glm::mat4 Camera::perspective_clip_t::projection() const {
    return glm::perspective(fov, aspect_ratio(), Z_NEAR, Z_FAR);
}

glm::mat4 Camera::perspective_clip_t::view(const glm::vec3& eye) const {
    return glm::lookAt(eye, look_at, UP);
}

}
