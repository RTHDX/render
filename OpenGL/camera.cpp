#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"


namespace opengl {

const glm::vec3 Camera::UP(0.0, 1.0, 0.0);
const glm::vec3 Camera::FORWARD(0.0, 0.0, -1.0);


Camera::Camera(float width, float height, float fov,
               const glm::vec3& position, const glm::vec3& look_at)
    : ui::Listener()
    , _width(width)
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

void Camera::consume(const ui::KeyEvent& event) {
    if (event.action == GLFW_RELEASE) { return; }
    switch (event.key) {
    case GLFW_KEY_W: move_forward(); break;
    case GLFW_KEY_S: move_backward(); break;
    case GLFW_KEY_A: move_left(); break;
    case GLFW_KEY_D: move_right(); break;
    case GLFW_KEY_Q: rotate_left(); break;
    case GLFW_KEY_E: rotate_right(); break;
    default:;
    }
}

void Camera::consume(const ui::MouseEvent& event) {}
void Camera::consume(const ui::MouseButtonEvent& event) {}
void Camera::consume(const ui::ScrollEvent& event) {}
void Camera::consume(const ui::DropEvent& event) {}

void Camera::move_forward() { _position.z -= 0.5f; }
void Camera::move_backward() { _position.z += 0.5f; }
void Camera::move_left() { _position.x -= 0.5f; _target.x -= 0.5f; }
void Camera::move_right() { _position.x += 0.5f; _target.x += 0.5f; }
void Camera::rotate_left() { _position.x -= 0.3f; }
void Camera::rotate_right() { _position.x += 0.3f; }

}
