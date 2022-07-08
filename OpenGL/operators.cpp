#include "item.hpp"
#include "operators.hpp"


namespace opengl {

static void move(Item& item, const glm::mat4& mat) {
    const auto& model = item.model();
    item.model(model * mat);
}

Rotate::Rotate(float rot)
    : Operator()
    , _mat(
        cos(rot),  0.0, sin(rot), 0.0,
        0.0,       1.0, 0.0, 0.0,
        -sin(rot), 0.0, cos(rot), 0.0,
        0.0, 0.0,  0.0, 1.0
    )
{}

void Rotate::accept(Item& item) { move(item, _mat); }


static glm::mat4 dir_to_mat(Move::Direction dir, float speed) {
    switch (dir) {
    case Move::Direction::FORWARD:
        return glm::mat4{
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, -speed,
            0.0, 0.0, 0.0, 1.0
        };
    case Move::Direction::BACKWARD:
        return glm::mat4{
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, speed,
            0.0, 0.0, 0.0, 1.0
        };
    case Move::Direction::LEFT:
        return glm::mat4{
            1.0, 0.0, 0.0, -speed,
            0.0, 1.0, 0.0,  0.0,
            0.0, 0.0, 1.0,  0.0,
            0.0, 0.0, 0.0,  1.0
        };
    case Move::Direction::RIGHT:
        return glm::mat4{
            1.0, 0.0, 0.0, speed,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0
        };
    }

    return glm::mat4(1.0);
}

Move::Move(Direction dir, float speed)
    : _mat(dir_to_mat(dir, speed))
{}

void Move::accept(Item& item) { move(item, _mat); }

}
