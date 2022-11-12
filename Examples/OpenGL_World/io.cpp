#include <iostream>

#include <glm/gtx/transform.hpp>

#include "io.hpp"


GlobalListener::GlobalListener(Item3D& item, ui::Publisher* publisher)
    : ui::Listener(publisher)
    , _item_listener(item)
{}

void GlobalListener::consume(const ui::KeyEvent& event) {
    if (event.key == GLFW_KEY_SPACE && event.action == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, _wire_mode ? GL_FILL : GL_LINE);
        _wire_mode = !_wire_mode;
        return;
    }

    _item_listener.consume(event);
}
void GlobalListener::consume(const ui::MouseEvent& event) {
    std::cout << event << std::endl;
    _item_listener.consume(event);
}
void GlobalListener::consume(const ui::MouseButtonEvent& event) {
    std::cout << event << std::endl;
    _item_listener.consume(event);
}
void GlobalListener::consume(const ui::ScrollEvent& event) {
    std::cout << event << std::endl;
    _item_listener.consume(event);
}
void GlobalListener::consume(const ui::DropEvent& event) {
    std::cout << event << std::endl;
    _item_listener.consume(event);
}


ItemListener::ItemListener(Item3D& item)
    : ui::Listener()
    , _item(item)
{}

void ItemListener::consume(const ui::KeyEvent& event) {
    const bool press_or_hold = event.action == GLFW_PRESS ||
                               event.action == GLFW_REPEAT;
    if (event.key == GLFW_KEY_W && press_or_hold) {
        return move_forward();
    }
    if (event.key == GLFW_KEY_S && press_or_hold) {
        return move_backward();
    }
    if (event.key == GLFW_KEY_A && press_or_hold) {
        return move_left();
    }
    if (event.key == GLFW_KEY_D && press_or_hold) {
        return move_right();
    }
    if (event.key == GLFW_KEY_Q && press_or_hold) {
        return rotate_left();
    }
    if (event.key == GLFW_KEY_E && press_or_hold) {
        return rotate_right();
    }
    std::cout << event << std::endl;
}

void ItemListener::consume(const ui::MouseEvent& event) {
    ;
}

void ItemListener::consume(const ui::MouseButtonEvent& event) {
    ;
}

void ItemListener::consume(const ui::ScrollEvent& event) {
    ;
}

void ItemListener::consume(const ui::DropEvent& event) {
    ;
}

void ItemListener::move_forward() {
    _item.modify(std::move(
        glm::translate(_item.model(), FORWARD_DIR)
    ));
}

void ItemListener::move_backward() {
    _item.modify(std::move(
        glm::translate(_item.model(), BACKWARD_DIR)
    ));
}

void ItemListener::move_left() {
    _item.modify(std::move(
        glm::translate(_item.model(), LEFT_DIR)
    ));
}

void ItemListener::move_right() {
    _item.modify(std::move(
        glm::translate(_item.model(), RIGHT_DIR)
    ));
}

void ItemListener::rotate_left() {
    _item.modify(std::move(
        glm::rotate(_item.model(), glm::radians(ROTATE_SPEED), ROTATE_AXE)
    ));
}

void ItemListener::rotate_right() {
    _item.modify(std::move(
        glm::rotate(_item.model(), -glm::radians(ROTATE_SPEED), ROTATE_AXE)
    ));
}
