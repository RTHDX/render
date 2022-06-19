#pragma once

#include <UI/property.hpp>
#include <UI/observer.hpp>
#include <UI/io.hpp>

#include "rtx_internal.hpp"

namespace rtx {

std::ostream& operator << (std::ostream& os, const glm::mat4&);

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

public:
    static constexpr float MOVE_SPEED = 5.5;

    Camera(const Point& position, const Point& target, float fov,
           size_t width, size_t height);

    Ray emit_ray(const size_t h_pos, const size_t w_pos) const;
    void move(Direction dir);

    size_t width() const { return _width; }
    size_t height() const { return _height; }
    const Point& position() const { return _position; }
    const Point& target() const { return _target; }

    float aspect_ratio() const {
        return float(_width) / float(_height);
    }

    glm::mat4 view() const;

private:
    Point ndc(size_t h_pos, size_t w_pos) const;
    Point pixel_screen(size_t h_pos, size_t w_pos) const;
    Point pixel_camera(size_t h_pos, size_t w_pos) const;

    glm::vec3 project_view_direction(const glm::vec3& direction) const;
    const glm::mat4& transform_mat(Direction) const;

    void dump() const;

private:
    Point _position, _target;
    float _field_of_view;
    size_t _width, _height;

    mutable glm::mat4 _view;
    mutable glm::mat4 _projection;
};


class CameraListener final : ui::Listener {
public:
    CameraListener(Camera& camera);

    void consume(const ui::KeyEvent& event) override;
    void consume(const ui::MouseEvent& event) override;
    void consume(const ui::MouseButtonEvent& event) override;
    void consume(const ui::ScrollEvent& event) override;
    void consume(const ui::DropEvent& event) override;

private:
    Camera& _camera;
};

}
