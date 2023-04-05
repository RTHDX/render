#pragma once

#include <glm/glm.hpp>

#include <UI/observer.hpp>


namespace opengl {

enum class Direction {
    NONE,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    ROTATE_LEFT,
    ROTATE_RIGHT,
    UP,
    DOWN
};

class Camera {
    static const glm::vec3 UP;
    static const glm::vec3 FORWARD;
    static constexpr float Z_NEAR = 0.1;
    static constexpr float Z_FAR = 100.0;

public:
    Camera(float width, float height, float fov,
           const glm::vec3& position = {0.0, 0.0, 5.0},
           const glm::vec3& look_at = {0.0, 0.0, 0.0});
    ~Camera() = default;

    float aspect_ratio() const { return _width / _height; }

    const glm::mat4& projection() const;
    glm::mat4 view() const;

    void move(Direction direction);

    void width(float width) { _width = width; eveal_projection(); }
    int width() const { return _width; }
    void height(float val) { _height = val; eveal_projection(); }
    int height() const { return _height; }

private:
    glm::vec3 up() const;
    glm::vec3 right() const;
    glm::vec3 direction() const;
    void eveal_projection();

private:
    float _width, _height, _fov;

    glm::mat4 _projection;
    glm::vec3 _position, _target;
};

class OrthoCamera final {
    static constexpr float Z_NEAR = 0.1;
    static constexpr float Z_FAR = 100.0;
    static constexpr glm::vec3 UP {0.0, 0.0, 1.0};

    struct ortho_clip_t {
        float factor;
        float width, height;
        float half_width() const { return (width * factor) / 2.0; }
        float half_height() const { return (height * factor) / 2.0; }
    };

public:
    OrthoCamera(const glm::vec3& pos, float width, float height,
                float factor = 1.0);

    glm::mat4 projection() const;
    glm::mat4 view() const;

    void zoom_in();
    void zoom_out();
    void zoom_step(float step);

private:
    glm::vec3 pos_;
    glm::mat4 projection_;
    glm::mat4 view_;
    float step_ = 0.01;
    ortho_clip_t clip_space_;
};


class CameraHandler final : public ui::Listener {
public:
    CameraHandler(Camera& camera);
    ~CameraHandler() override = default;

    const Camera& impl() const { return _camera; }

    virtual void consume(const ui::KeyEvent& event);
    virtual void consume(const ui::MouseEvent& event);
    virtual void consume(const ui::MouseButtonEvent& event);
    virtual void consume(const ui::ScrollEvent& event);
    virtual void consume(const ui::DropEvent& event);
    virtual void consume(const ui::FramebufferEvent& event);

private:
    Camera& _camera;
};

}
