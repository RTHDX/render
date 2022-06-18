#include <cmath>
#include <GLFW/glfw3.h>
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
    return Ray(_position, direction);
}

void Camera::move(Direction direction) {
    const auto& transform = transform_mat(direction);
    glm::vec4 res = transform * glm::vec4(_position, 1.0);
    _position = glm::vec3(res.x, res.y, res.z);
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

glm::mat4 Camera::view() const {
    return glm::transpose(glm::lookAt(_position, _target, UP));
}

glm::vec3 Camera::project_view_direction(const glm::vec3& direction) const {
    glm::vec4 temp(direction.x, direction.y, direction.z, 1.0);
    glm::vec4 res = view() * temp;
    return glm::normalize(glm::vec3(res.x, res.y, res.z));
}

const glm::mat4& Camera::transform_mat(Direction dir) const {
    float rot = glm::radians(10.0f);
    switch (dir) {
    case Direction::FORWARD: {
        static glm::mat4 mat {
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 1.0,
            0.0, 0.0, 0.0, 1.0
        };
        return mat;
    } case Direction::BACKWARD: {
        static glm::mat4 mat {
            1.0, 0.0, 0.0,  0.0,
            0.0, 1.0, 0.0,  0.0,
            0.0, 0.0, 1.0, -1.0,
            0.0, 0.0, 0.0,  1.0
        };
        return mat;
    } case Direction::LEFT: {
        static glm::mat4 mat {
            1.0, 0.0, 0.0, -1.0,
            0.0, 1.0, 0.0,  0.0,
            0.0, 0.0, 1.0,  0.0,
            0.0, 0.0, 0.0,  1.0
        };
        return mat;
    } case Direction::RIGHT: {
        static glm::mat4 mat {
            1.0, 0.0, 0.0, 1.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0
        };
        return mat;
    } case Direction::ROTATE_LEFT: {
        static glm::mat4 mat {
            cos(rot),  0.0, sin(rot), 0.0,
            0.0,       1.0, 0.0,      0.0,
            -sin(rot), 0.0, cos(rot), 0.0,
            0.0,       0.0, 0.0,      1.0
        };
        return mat;
    } case Direction::ROTATE_RIGHT: {
        static glm::mat4 mat {
            -cos(rot), 0.0, -sin(rot), 0.0,
            0.0,       1.0, 0.0,       0.0,
            sin(rot),  0.0, -cos(rot), 0.0,
            0.0,       0.0, 0.0,       1.0
        };
        return mat;
    } default:;
    }
    static glm::mat4 one(1.0);
    return one;
}


CameraListener::CameraListener(Camera& camera)
    : ui::Listener(&ui::io::IO::instance())
    , _camera(camera)
{}

void CameraListener::consume(const ui::KeyEvent& event) {
    if (event.action != GLFW_PRESS) return;
    switch (event.key) {
    case GLFW_KEY_W:
        _camera.move(Direction::FORWARD);
        break;
    case GLFW_KEY_S:
        _camera.move(Direction::BACKWARD);
        break;
    case GLFW_KEY_A:
        _camera.move(Direction::LEFT);
        break;
    case GLFW_KEY_D:
        _camera.move(Direction::RIGHT);
        break;
    case GLFW_KEY_Q:
        _camera.move(Direction::ROTATE_LEFT);
        break;
    case GLFW_KEY_E:
        _camera.move(Direction::ROTATE_RIGHT);
        break;
    default:;
    }
}

void CameraListener::consume(const ui::MouseEvent& event) {
    ;
}

void CameraListener::consume(const ui::MouseButtonEvent& event) {
    ;
}

void CameraListener::consume(const ui::ScrollEvent& event) {
    ;
}

void CameraListener::consume(const ui::DropEvent& event) {
    ;
}

}
