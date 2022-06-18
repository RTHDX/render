#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"

namespace rtx {

const glm::vec3 Camera::UP(0.0, 1.0, 0.0);

Camera::Camera(const Point& position, const Point& target, float fov,
               size_t width, size_t height)
    : _position(position)
    , _target(target)
    , _field_of_view(fov)
    , _width(width)
    , _height(height)

    , _view(glm::mat4(1.0))
    , _projection(glm::mat4(1.0))
{}

Ray Camera::emit_ray(const size_t h_pos, const size_t w_pos) const {
    glm::vec3 direction = project_view_direction(glm::normalize(
        pixel_camera(h_pos, w_pos)
    ));
    //return Ray(_position, glm::normalize(pixel_camera(h_pos, w_pos)));
    return Ray(_position, direction);
}

void Camera::move(Direction direction) {
    switch (direction) {
    case Direction::FORWARD: break;
    case Direction::BACKWARD: break;
    case Direction::LEFT: break;
    case Direction::RIGHT: break;
    case Direction::ROTATE_LEFT: break;
    case Direction::ROTATE_RIGHT: break;
    }
}

Point Camera::ndc(size_t h_pos, size_t w_pos) const {
    return Point(
        (w_pos + 0.5) / _width,
        (h_pos + 0.5) / _height,
        -1
    );
}

Point Camera::pixel_screen(size_t h_pos, size_t w_pos) const {
    auto ndc = this->ndc(h_pos, w_pos);
    return Point(
        2 * ndc.x - 1,
        2 * ndc.y - 1,
        ndc.z
    );
}

Point Camera::pixel_camera(size_t h_pos, size_t w_pos) const {
    auto pixel_screen = this->pixel_screen(h_pos, w_pos);
    float ftan = tan(_field_of_view / 2.0f);
    return Point(
        pixel_screen.x * aspect_ratio() * ftan,
        pixel_screen.y * ftan,
        pixel_screen.z
    );
}

const glm::mat4& Camera::view() const {
    _view = glm::transpose(glm::lookAt(_position, _target, UP));
    return _view;
}

const glm::mat4& Camera::projection() const {
    _projection = glm::transpose(glm::perspective(_field_of_view,
                                                  aspect_ratio(), 0.1f,
                                                  100.0f));
    return _projection;
}

glm::vec3 Camera::project_view_direction(const glm::vec3& direction) const {
    glm::vec4 temp(direction.x, direction.y, direction.z, 1.0);
    glm::vec4 res = view() * temp;
    return glm::normalize(glm::vec3(res.x, res.y, res.z));
}

}
