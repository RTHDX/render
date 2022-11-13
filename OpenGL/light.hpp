#pragma once

#include <glm/glm.hpp>


namespace opengl {

class Light {
public:
    Light(glm::vec3&& pos, glm::vec4&& col);

    const glm::vec3& position() const { return _position; }
    const glm::vec4& color() const { return _color; }

private:
    glm::vec3 _position;
    glm::vec4 _color;
};

}
