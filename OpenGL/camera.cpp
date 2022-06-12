#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"


namespace opengl {

const glm::vec3 Camera::UP(0.0, 1.0, 0.0);
const glm::vec3 Camera::FORWARD(0.0, 0.0, -1.0);


Camera::Camera(float width, float height, float fov,
               const glm::vec3& position, const glm::vec3& look_at)
    : _width(width)
    , _height(height)
    , _projection(glm::perspective(fov, aspect_ratio(), 0.1f, 100.0f))
    , _position(position)
    , _target(look_at)
{}

const glm::mat4& Camera::projection() const {
    return _projection;
}

glm::mat4 Camera::view() const {
    return glm::lookAt(_position, _target, up());
}

glm::vec3 Camera::up() const {
    return glm::normalize(glm::cross(direction(), right()));
}

glm::vec3 Camera::right() const {
    return glm::normalize(glm::cross(UP, direction()));
}

glm::vec3 Camera::direction() const {
    return glm::normalize(_position - _target);
}

}
