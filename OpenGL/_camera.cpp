#include <glm/gtc/matrix_transform.hpp>

#include "_camera.hpp"


namespace opengl {


template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;


_Camera _Camera::create_perspective(float width,
                                    float height,
                                    float field_of_view,
                                    const glm::vec3& pos,
                                    const glm::vec3& look_at) {
    perspective_clip_t clip;
    clip.width   = width;
    clip.height  = height;
    clip.fov     = field_of_view;
    clip.look_at = look_at;
    _Camera cam;
    cam.clip_ = std::move(clip);
    cam.pos_  = pos;
    return cam;
}

_Camera _Camera::create_topdown(float width,
                                float height,
                                float factor,
                                const glm::vec3& pos) {
    ortho_clip_t clip;
    clip.width   = width;
    clip.height  = height;
    clip.factor  = factor;
    clip.look_at = {pos.x, pos.y - 1.0, pos.z};
    _Camera cam;
    cam.clip_ = std::move(clip);
    cam.pos_ = pos;
    return cam;
}

glm::mat4 _Camera::projection() const {
    switch (mode()) {
    case Mode::PERSPECTIVE:
        return std::get<perspective_clip_t>(clip_).projection();
    case Mode::TOP_DOWN:
        return std::get<ortho_clip_t>(clip_).projection();
    default: return glm::mat4(1.0);
    }
}

glm::mat4 _Camera::view() const {
    switch (mode()) {
    case Mode::PERSPECTIVE:
        return std::get<perspective_clip_t>(clip_).view(pos_);
    case Mode::TOP_DOWN:
        return std::get<ortho_clip_t>(clip_).view(pos_);
    default: return glm::mat4(1.0);
    }
}

glm::mat4 _Camera::ipv() const {
    return glm::inverse(projection() * view());
}

const glm::vec3& _Camera::position() const {
    return pos_;
}

void _Camera::update_viewport(const glm::vec2& viewport) {
    std::visit(overloaded {
        [viewport](ortho_clip_t& clip) {
            clip.width = viewport.x;
            clip.height = viewport.y;
        }, [viewport](perspective_clip_t& clip) {
            clip.width = viewport.x;
            clip.height = viewport.y;
        }
    }, clip_);
}

_Camera::Mode _Camera::mode() const {
    if (std::holds_alternative<ortho_clip_t>(clip_)) {
        return Mode::TOP_DOWN;
    }
    if (std::holds_alternative<perspective_clip_t>(clip_)) {
        return Mode::PERSPECTIVE;
    }

    return Mode::NONE;
}

float _Camera::ortho_clip_t::half_width() const {
    return (width * factor) / 2.0;
}

float _Camera::ortho_clip_t::half_height() const {
    return (height * factor) / 2.0;
}

glm::mat4 _Camera::ortho_clip_t::projection() const {
    return glm::ortho(
        half_width(),  -half_width(),
        half_height(), -half_height(),
        Z_NEAR, Z_FAR
    );
}

glm::mat4 _Camera::ortho_clip_t::view(const glm::vec3& eye) const {
    return glm::lookAt(eye, look_at, UP);
}

float _Camera::perspective_clip_t::aspect_ratio() const {
    return width / height;
}

glm::mat4 _Camera::perspective_clip_t::projection() const {
    return glm::perspective(fov, aspect_ratio(), Z_NEAR, Z_FAR);
}

glm::mat4 _Camera::perspective_clip_t::view(const glm::vec3& eye) const {
    return glm::lookAt(eye, look_at, UP);
}

}
