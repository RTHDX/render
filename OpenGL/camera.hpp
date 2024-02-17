#pragma once

#include <numeric>
#include <variant>

#include <glm/glm.hpp>


namespace opengl {

class Camera final {
public:
    static Camera create_perspective(
        float width,
        float height,
        float field_of_view,
        const glm::vec3& pos,
        const glm::vec3& look_at
    );

    static Camera create_topdown(
        float width,
        float height,
        float factor,
        const glm::vec3& pos
    );

    static Camera create_topdown(
        float width,
        float height,
        const glm::vec3& pos
    );

    glm::mat4 projection() const;
    glm::mat4 view() const;
    glm::mat4 ipv() const;

    glm::ivec4 viewport() const;
    const glm::vec3& position() const;

    void update_viewport(const glm::vec2& viewport);

    void zoom_in();
    void zoom_out();

private:
    static constexpr float Z_NEAR = 0.01;
    static constexpr float Z_FAR = 100.0;

    struct ortho_clip_t final {
        static constexpr glm::vec3 UP {0.0, 0.0, 1.0};

        float factor, width, height;
        glm::vec3 look_at;

        float half_width() const;
        float half_height() const;

        glm::mat4 projection() const;
        glm::mat4 view(const glm::vec3& eye) const;
    };

    struct perspective_clip_t final {
        static constexpr glm::vec3 UP      {0.0, 1.0,  0.0};
        static constexpr glm::vec3 FORWARD {0.0, 0.0, -1.0};

        float width, height, fov;
        glm::vec3 look_at;

        float aspect_ratio() const;
        glm::mat4 projection() const;
        glm::mat4 view(const glm::vec3& eye) const;
    };

    using clip_t = std::variant<ortho_clip_t, perspective_clip_t>;

    enum Mode {
        NONE = 0,
        PERSPECTIVE,
        TOP_DOWN
    };

private:
    Camera() = default;

    Mode mode() const;

private:
    clip_t clip_;
    glm::vec3 pos_;
};

}
