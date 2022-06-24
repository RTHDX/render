#include <cmath>
#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
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
    auto raw_directin = pixel_camera(h_pos, w_pos);
    auto direction = project_view_direction(glm::normalize(raw_directin));
    return Ray(position(), direction);
}

void Camera::move(Direction direction) {
    const auto& transform = glm::transpose(transform_mat(direction));

    _position = transform * glm::vec4(_position, 1.0);
    if (direction == Direction::FORWARD ||
        direction == Direction::BACKWARD ||
        direction == Direction::LEFT ||
        direction == Direction::RIGHT) {
        _target = transform * glm::vec4(_target, 1.0);
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

glm::mat4 Camera::view() const {
    glm::mat4 out = glm::transpose(glm::lookAt(position(), target(), UP));
    return out;
}

glm::vec3 Camera::project_view_direction(const glm::vec3& direction) const {
    glm::vec4 res = view() * glm::vec4(direction, 1.0f);
    return glm::normalize(glm::vec3(res));
}

const glm::mat4& Camera::transform_mat(Direction dir) const {
    float rot = glm::radians(10.0f);
    switch (dir) {
    case Direction::FORWARD: {
        static glm::mat4 mat {
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, -MOVE_SPEED,
            0.0, 0.0, 0.0, 1.0
        };
        return mat;
    } case Direction::BACKWARD: {
        static glm::mat4 mat {
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, MOVE_SPEED,
            0.0, 0.0, 0.0, 1.0
        };
        return mat;
    } case Direction::LEFT: {
        static glm::mat4 mat {
            1.0, 0.0, 0.0, -MOVE_SPEED,
            0.0, 1.0, 0.0,  0.0,
            0.0, 0.0, 1.0,  0.0,
            0.0, 0.0, 0.0,  1.0
        };
        return mat;
    } case Direction::RIGHT: {
        static glm::mat4 mat {
            1.0, 0.0, 0.0, MOVE_SPEED,
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
            cos(-rot),   0.0, sin(-rot), 0.0,
            0.0,         1.0, 0.0,       0.0,
            -sin(-rot),  0.0, cos(-rot), 0.0,
            0.0,         0.0, 0.0,       1.0
        };
        return mat;
    } default:;
    }
    static glm::mat4 one(1.0);
    return one;
}

void Camera::dump() const {
    std::cout << "Position: " << glm::to_string(_position) << '\n'
              << "Target: " << glm::to_string(_target) << '\n'
              << "View:\n" << view()
              << std::endl;
}


CameraListener::CameraListener(Camera& camera)
    : ui::Listener(&ui::io::IO::instance())
    , _camera(camera)
{}

void CameraListener::consume(const ui::KeyEvent& event) {
    if (event.action == GLFW_RELEASE) return;

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

std::ostream& operator << (std::ostream& os, const glm::mat4& mat) {
    static constexpr char TEMPL[] =
        "({:6.4f};{:6.4f};{:6.4f};{:6.4f}\n"
        " {:6.4f};{:6.4f};{:6.4f};{:6.4f}\n"
        " {:6.4f};{:6.4f};{:6.4f};{:6.4f}\n"
        " {:6.4f};{:6.4f};{:6.4f};{:6.4f})";
    os << std::format(TEMPL,
        mat[0][0], mat[0][1], mat[0][2], mat[0][3],
        mat[1][0], mat[1][1], mat[1][2], mat[1][3],
        mat[2][0], mat[2][1], mat[2][2], mat[2][3],
        mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
    return os;
}

}
