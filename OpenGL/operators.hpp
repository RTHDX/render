#pragma once

#include <glm/glm.hpp>

#include "operable.hpp"

namespace opengl {

class Rotate : Operator {
public:
    Rotate(float rot);

    void accept(Item& item);

private:
    glm::mat4 _mat;
};

class Move : Operator {
public:
    enum class Direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    Move(Direction dir, float speed);

    void accept(Item& item);

private:
    glm::mat4 _mat;
};

}
