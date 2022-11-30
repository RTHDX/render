#include <format>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "io.hpp"
#include "map_reader.hpp"

static bool is_key_press_or_hold(const ui::KeyEvent& event, int key);


GlobalListener::GlobalListener(Scene& scene, ui::Publisher* publisher)
    : ui::Listener(publisher)
    , _scene(scene)
    , _item_listener(scene.items()[0])
    , _camera_listener(scene.camera())
{}

void GlobalListener::consume(const ui::KeyEvent& event) {
    if (event.key == GLFW_KEY_SPACE && event.action == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, _wire_mode ? GL_FILL : GL_LINE);
        _wire_mode = !_wire_mode;
        return;
    }

    return is_item_active() ?
        _item_listener.consume(event) :
        _camera_listener.consume(event);
}
void GlobalListener::consume(const ui::MouseEvent& event) {
    _last_mouse_event = event;

    return is_item_active() ?
        _item_listener.consume(event) :
        _camera_listener.consume(event);
}
void GlobalListener::consume(const ui::MouseButtonEvent& event) {
    if (event.button == GLFW_MOUSE_BUTTON_LEFT && event.action == GLFW_PRESS) {
        return pick_pixel();
    }

    return is_item_active() ?
        _item_listener.consume(event) :
        _camera_listener.consume(event);
}

void GlobalListener::consume(const ui::ScrollEvent& event) {
    return is_item_active() ?
        _item_listener.consume(event) :
        _camera_listener.consume(event);
}

void GlobalListener::consume(const ui::DropEvent& event) {
    std::cout << event << std::endl;
    if (event.paths.size() == 0) { return; }

    map::reader::Reader reader(event.paths[0]);
    if (!reader.verify()) {
        std::wcerr << "Dropped file has incorrect extenstion" << std::endl;
    }

    reader.read();
}

void GlobalListener::pick_pixel() {
    GLuint index;
    int x = _last_mouse_event.xpos;
    int y = _scene.height() - (int)_last_mouse_event.ypos - 1;
    SAFE_CALL(glReadPixels(
        x, y,
        1, 1,
        GL_STENCIL_INDEX,
        GL_UNSIGNED_INT,
        &index
    ));
    _scene.activate_index(index);
}

bool GlobalListener::is_item_active() {
    return _item_listener.item().is_active();
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


CameraListener::CameraListener(opengl::Camera& camera)
    : ui::Listener()
    , _camera(camera)
{}

void CameraListener::consume(const ui::KeyEvent& event) {
    if (is_key_press_or_hold(event, GLFW_KEY_W)) {
        return _camera.move(opengl::Direction::FORWARD);
    }
    if (is_key_press_or_hold(event, GLFW_KEY_S)) {
        return _camera.move(opengl::Direction::BACKWARD);
    }
    if (is_key_press_or_hold(event, GLFW_KEY_A)) {
        return _camera.move(opengl::Direction::LEFT);
    }
    if (is_key_press_or_hold(event, GLFW_KEY_D)) {
        return _camera.move(opengl::Direction::RIGHT);
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


static bool is_key_press_or_hold(const ui::KeyEvent& event, int key) {
    return event.key == key &&
           (event.action == GLFW_PRESS || event.action == GLFW_REPEAT);
}
