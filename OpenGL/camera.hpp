#pragma once

#include <glm/glm.hpp>


namespace opengl {

class Camera {
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

private:
    glm::vec3 up() const;
    glm::vec3 right() const;
    glm::vec3 direction() const;

private:
    float _width, _height, _fov;

    glm::mat4 _projection;
    glm::vec3 _position, _target;
};

}
