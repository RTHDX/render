#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <UI/io.hpp>

#include "item.hpp"


class ItemListener : public ui::Listener {
    static constexpr float SPEED = 0.2;
    static constexpr float ROTATE_SPEED = 5;
    static constexpr glm::vec3 FORWARD_DIR  {SPEED,  0.0,    0.0};
    static constexpr glm::vec3 BACKWARD_DIR {-SPEED, 0.0,    0.0};
    static constexpr glm::vec3 LEFT_DIR     {0.0,    0.0,    SPEED};
    static constexpr glm::vec3 RIGHT_DIR    {0.0,    0.0,    -SPEED};
    static constexpr glm::vec3 ROTATE_AXE   {0.0,    1.0,    0.0};

public:
    ItemListener(Item3D& item);

    void consume(const ui::KeyEvent& event) override;
    void consume(const ui::MouseEvent& event) override;
    void consume(const ui::MouseButtonEvent& event) override;
    void consume(const ui::ScrollEvent& event) override;
    void consume(const ui::DropEvent& event) override;

    const Item3D& item() const { return _item; }

private:
    void move_forward();
    void move_backward();
    void move_left();
    void move_right();
    void rotate_left();
    void rotate_right();

private:
    Item3D& _item;
};

class CameraListener : public ui::Listener {
public:
    CameraListener(opengl::Camera& camera);

    void consume(const ui::KeyEvent& event) override;
    void consume(const ui::MouseEvent& event) override;
    void consume(const ui::MouseButtonEvent& event) override;
    void consume(const ui::ScrollEvent& event) override;
    void consume(const ui::DropEvent& event) override;

    const opengl::Camera& camera() const { return _camera; }

private:
    opengl::Camera& _camera;
};

class GlobalListener : public ui::Listener {
public:
    GlobalListener(Scene& scene, ui::Publisher* publisher);

    void consume(const ui::KeyEvent& event) override;
    void consume(const ui::MouseEvent& event) override;
    void consume(const ui::MouseButtonEvent& event) override;
    void consume(const ui::ScrollEvent& event) override;
    void consume(const ui::DropEvent& event) override;

private:
    void pick_pixel();
    bool is_item_active();

private:
    ItemListener _item_listener;
    CameraListener _camera_listener;

    bool _wire_mode = false;
    Scene& _scene;
    ui::MouseEvent _last_mouse_event;
};
