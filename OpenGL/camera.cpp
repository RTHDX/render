#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <UI/io.hpp>

#include "camera.hpp"


namespace opengl {

const glm::vec3 Camera::UP(0.0, 1.0, 0.0);
const glm::vec3 Camera::FORWARD(0.0, 0.0, -1.0);

static float MOVE_SPEED = 0.1;
static float ROTATION_SPEED = 10;
static glm::mat4 move_modifiers(Direction dir);
static Direction convert(int key);


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

void Camera::move(Direction direction) {
    auto mat = move_modifiers(direction);
    _position = glm::vec4(_position, 1.0) * mat;
    if (direction == Direction::FORWARD || direction == Direction::BACKWARD ||
        direction == Direction::LEFT || direction == Direction::RIGHT ||
        direction == Direction::UP || direction == Direction::DOWN) {
        _target = glm::vec4(_target, 1.0) * mat;
    }
}


CameraHandler::CameraHandler(Camera& camera)
    : ui::Listener(&ui::io::IO::instance())
    , _camera(camera)
{}

void CameraHandler::consume(const ui::KeyEvent& event) {
    if (event.action == GLFW_RELEASE) { return; }

    _camera.move(convert(event.key));
}

void CameraHandler::consume(const ui::MouseEvent& event) {}
void CameraHandler::consume(const ui::MouseButtonEvent& event) {}

void CameraHandler::consume(const ui::ScrollEvent& event) {
    Direction direction = event.yoffset > 0 ? Direction::UP :
                                              Direction::DOWN;
    _camera.move(direction);
}

void CameraHandler::consume(const ui::DropEvent& event) {}

void CameraHandler::consume(const ui::FramebufferEvent&) {}

static glm::mat4 move_modifiers(Direction dir) {
    float rot = glm::radians(ROTATION_SPEED);
    switch (dir) {
    case Direction::FORWARD:
        return glm::mat4 {
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, -MOVE_SPEED,
            0.0, 0.0, 0.0, 1.0
        };
    case Direction::BACKWARD:
        return glm::mat4 {
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, MOVE_SPEED,
            0.0, 0.0, 0.0, 1.0
        };
    case Direction::LEFT:
        return glm::mat4 {
            1.0, 0.0, 0.0, -MOVE_SPEED,
            0.0, 1.0, 0.0,  0.0,
            0.0, 0.0, 1.0,  0.0,
            0.0, 0.0, 0.0,  1.0
        };
    case Direction::RIGHT:
        return glm::mat4 {
            1.0, 0.0, 0.0, MOVE_SPEED,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0
        };
    case Direction::ROTATE_LEFT:
        return glm::mat4 {
            cos(rot),  0.0, sin(rot), 0.0,
            0.0,       1.0, 0.0,      0.0,
            -sin(rot), 0.0, cos(rot), 0.0,
            0.0,       0.0, 0.0,      1.0
        };
    case Direction::ROTATE_RIGHT:
        return glm::mat4 {
            cos(-rot),   0.0, sin(-rot), 0.0,
            0.0,         1.0, 0.0,       0.0,
            -sin(-rot),  0.0, cos(-rot), 0.0,
            0.0,         0.0, 0.0,       1.0
        };
    case Direction::UP:
        return glm::mat4 {
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, MOVE_SPEED,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0
        };
    case Direction::DOWN:
        return glm::mat4{
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, -MOVE_SPEED,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0
        };
    default:;
    }
    return glm::mat4(1.0);
}

static Direction convert(int key) {
    switch (key) {
    case GLFW_KEY_W: return Direction::FORWARD;
    case GLFW_KEY_S: return Direction::BACKWARD;
    case GLFW_KEY_A: return Direction::LEFT;
    case GLFW_KEY_D: return Direction::RIGHT;
    case GLFW_KEY_Q: return Direction::ROTATE_LEFT;
    case GLFW_KEY_E: return Direction::ROTATE_RIGHT;
    }
    return Direction::NONE;
}

}
