#include "light.hpp"


namespace opengl {

Light::Light(glm::vec3&& pos, glm::vec4&& col)
    : _position(std::move(pos))
    , _color(std::move(col))
{}

}
