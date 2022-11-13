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

class GlobalListener : public ui::Listener {
public:
    GlobalListener(Item3D& item, ui::Publisher* publisher);

    void consume(const ui::KeyEvent& event) override;
    void consume(const ui::MouseEvent& event) override;
    void consume(const ui::MouseButtonEvent& event) override;
    void consume(const ui::ScrollEvent& event) override;
    void consume(const ui::DropEvent& event) override;

private:
    void pick_pixel();

private:
    ItemListener _item_listener;

    bool _wire_mode = false;
    ui::MouseEvent _last_mouse_event;
};
