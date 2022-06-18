#pragma once

#include "rtx_internal.hpp"

namespace rtx {

enum class Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    ROTATE_LEFT,
    ROTATE_RIGHT
};

class Camera {
    static const glm::vec3 UP;
    static constexpr float MOVE_SPEED     = 0.5;
    static constexpr float ROTATION_SPEED = 0.5;

public:
    Camera(const Point& position, const Point& target, float fov,
           size_t width, size_t height);

    Ray emit_ray(const size_t h_pos, const size_t w_pos) const;

    size_t width() const { return _width; }
    size_t height() const { return _height; }

    float aspect_ratio() const {
        return float(_width) / float(_height);
    }

    void move(Direction dir);

private:
    Point ndc(size_t h_pos, size_t w_pos) const;
    Point pixel_screen(size_t h_pos, size_t w_pos) const;
    Point pixel_camera(size_t h_pos, size_t w_pos) const;

    const glm::mat4& view() const;
    const glm::mat4& projection() const;
    glm::vec3 project_view_direction(const glm::vec3& direction) const;

private:
    Point _position, _target;
    float _field_of_view;
    size_t _width, _height;

    mutable glm::mat4 _view;
    mutable glm::mat4 _projection;
};

}
