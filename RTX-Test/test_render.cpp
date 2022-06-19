#include <iostream>

#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <RTX/camera.hpp>
#include <RTX/render.hpp>

constexpr size_t TEST_WIDTH = 200;
constexpr size_t TEST_HEIGHT = 200;
constexpr rtx::Point START_POS{0.0, 0.0, 10.0};
constexpr rtx::Point START_TAR{0.0, 0.0, 0.0};

std::vector<rtx::Sphere> make_spheres() {
    return {
        rtx::Sphere(
            {0.0, 0.0, 0.0},
            rtx::Material(
                {0.9, 0.1, 0.0, 0.0},
                rtx::Color(0.3, 0.1, 0.1),
                10.0f,
                1.0),
            5.0
        )
    };
}

std::vector<rtx::Light> make_lights() {
    return {
        rtx::Light({0.0, 20.0, 0.0}, 5.0f)
    };
}

rtx::Scene make_scene() {
    return rtx::Scene(
        make_spheres(),
        make_lights()
    );
}

rtx::Camera& make_camera() {
    static rtx::Camera camera (
        {0.0, 0.0, 10.0},
        {0.0, 0.0, 0.0},
        glm::radians(90.0f),
        TEST_WIDTH,
        TEST_HEIGHT
    );
    return camera;
}

class RenderTest : public testing::Test {
public:
    RenderTest()
        : testing::Test()
        , render(make_scene(), {0.1, 0.1, 0.1}, make_camera())
    {}

public:
    rtx::Render render;
};

bool operator == (const rtx::Framebuffer& lhs, const rtx::Framebuffer& rhs) {
    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i]) { return false; }
    }
    return true;
}

bool operator != (const rtx::Framebuffer& lhs, const rtx::Framebuffer& rhs) {
    return !(lhs == rhs);
}


TEST_F(RenderTest, move_camera) {
    auto& camera = make_camera();
    const auto& buffer_before = render.buffer();
    render.render();
    const auto& buffer_after = render.buffer();
    ASSERT_NE(buffer_before, buffer_after);
}
