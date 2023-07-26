#pragma once

#include <limits>
#include <glm/glm.hpp>

#include <UI/observer.hpp>


namespace opengl {

enum class Direction {
    NONE,
    FORWARD,
    FORWARD_LEFT,
    FORWARD_RIGHT,
    BACKWARD,
    BACKWARD_LEFT,
    BACKWARD_RIGHT,
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

    void update_sizes(size_t w, size_t h) {
        _width  = float(w);
        _height = float(h);
        eveal_projection();
    }

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
    static constexpr float Z_NEAR = 0.0001;
    static constexpr float Z_FAR = 10000.0;
    static constexpr glm::vec3 UP {0.0, 0.0, 1.0};

    static constexpr float sq_2 = 1.414213562;
    static constexpr glm::vec3 NORTH      = {0.0,   0.0, -1.0};
    static constexpr glm::vec3 EAST_NORTH = {sq_2,  0.0, -sq_2};
    static constexpr glm::vec3 EAST       = {1.0,   0.0, 0.0};
    static constexpr glm::vec3 EAST_SOUTH = {sq_2,  0.0, sq_2};
    static constexpr glm::vec3 SOUTH      = {0.0,   0.0, 1.0};
    static constexpr glm::vec3 WEST_SOUTH = {-sq_2, 0.0, sq_2};
    static constexpr glm::vec3 WEST       = {-1.0,  0.0, 0.0};
    static constexpr glm::vec3 WEST_NORTH = {-sq_2, 0.0, -sq_2};
    static constexpr glm::vec3 UP_MOVE    = {0.0,   1.0, 0.0};
    static constexpr glm::vec3 DOWN_MOVE  = {0.0,  -1.0, 0.0};

    struct ortho_clip_t {
        float factor = 0.0f;
        float width = 0.0f, height = 0.0;

        float half_width() const { return (width * factor) / 2.0; }
        float half_height() const { return (height * factor) / 2.0; }
        bool is_empty() const {
            const auto e = std::numeric_limits<float>::epsilon();
            return (
                std::fabs(factor - 0) <= e * std::fmax(std::fabs(factor), 0) &&
                std::fabs(width - 0)  <= e * std::fmax(std::fabs(width),  0) &&
                std::fabs(height - 0) <= e * std::fmax(std::fabs(height), 0)
            );
        }
    };

public:
    OrthoCamera() = default;
    OrthoCamera(const glm::vec3& pos, float width, float height,
                float factor = 1.0);
    OrthoCamera(const OrthoCamera&) = default;
    OrthoCamera& operator = (const OrthoCamera&) = default;
    OrthoCamera(OrthoCamera&&) = default;
    OrthoCamera& operator = (OrthoCamera&&) = default;
    ~OrthoCamera() = default;

    const glm::mat4& projection() const;
    const glm::mat4& view() const;
    const glm::vec3& position() const { return pos_; }
    const glm::mat4& ipv() const;
    const glm::vec4& viewport() const;

    void move(Direction dir);
    void zoom_in();
    void zoom_out();
    void zoom_step(float step);
    float zoom_step() const;
    void update_sizes(size_t w, size_t h);
    glm::vec3 screen_to_world(const glm::ivec2& screen);
    glm::vec2 world_pixel_size() const;

    float clip_space_factor() const;

    bool is_empty() const {
        return pos_        == glm::vec3{0.0, 0.0, 0.0} &&
               projection_ == glm::mat4(1.0) &&
               view_       == glm::mat4(1.0) &&
               viewport_   == glm::vec4(0, 0, 0, 0) &&
               clip_space_.is_empty();
    }

private:
    void update_projection();
    void update_view();
    void update_ipv();
    void update_viewport(size_t w, size_t h);

private:
    glm::vec3 pos_        = {0.0, 0.0, 0.0};
    glm::vec4 viewport_   = {0, 0, 0, 0};
    glm::mat4 projection_ = glm::mat4{1.0};
    glm::mat4 view_       = glm::mat4{1.0};
    glm::mat4 ipv_        = glm::mat4(1.0);
    float step_           = 0.001;
    ortho_clip_t clip_space_;
};


class CameraHandler final : public ui::Listener {
public:
    CameraHandler(Camera& camera);
    ~CameraHandler() override = default;

    const Camera& impl() const { return _camera; }

    void consume(const ui::KeyEvent& event) override;
    void consume(const ui::MouseEvent& event) override;
    void consume(const ui::MouseButtonEvent& event) override;
    void consume(const ui::ScrollEvent& event) override;
    void consume(const ui::DropEvent& event) override;
    void consume(const ui::FramebufferEvent& event) override;

private:
    Camera& _camera;
};

}
