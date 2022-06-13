#pragma once

#include <glm/glm.hpp>

#include <observer.hpp>


namespace opengl {

class Camera final : public ui::Listener {
    static const glm::vec3 UP;
    static const glm::vec3 FORWARD;

public:
    Camera(float width, float height, float fov,
           const glm::vec3& position = {0.0, 0.0, 5.0},
           const glm::vec3& look_at = {0.0, 0.0, 0.0});
    ~Camera() = default;

    float aspect_ratio() const { return _width / _height; }

    const glm::mat4& projection() const;
    glm::mat4 view() const;

    virtual void consume(const ui::KeyEvent& event);
    virtual void consume(const ui::MouseEvent& event);
    virtual void consume(const ui::MouseButtonEvent& event);
    virtual void consume(const ui::ScrollEvent& event);
    virtual void consume(const ui::DropEvent& event);

private:
    glm::vec3 up() const;
    glm::vec3 right() const;
    glm::vec3 direction() const;

    void move_forward();
    void move_backward();
    void move_left();
    void move_right();
    void rotate_left();
    void rotate_right();

private:
    float _width, _height, _fov;

    glm::mat4 _projection;
    glm::vec3 _position, _target;
};

}
