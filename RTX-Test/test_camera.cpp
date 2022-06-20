#include <iostream>

#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <RTX/camera.hpp>


constexpr size_t TEST_WIDTH    = 200;
constexpr size_t TEST_HEIGHT   = 200;
constexpr rtx::Point START_POS {0.0, 0.0, 10.0};
constexpr rtx::Point START_TAR {0.0, 0.0, 0.0};

class CameraTest : public testing::Test {
public:
    CameraTest()
        : testing::Test()
        , camera(START_POS, START_TAR, glm::radians(45.0f), TEST_WIDTH,
                 TEST_HEIGHT)
        , sphere({0.0, 0.0, 0.0}, rtx::Material(), 5.0)
    {}

    rtx::Ray center_ray() const {
        auto ray = camera.emit_ray(TEST_HEIGHT / 2, TEST_WIDTH / 2);
        return ray;
    }

    testing::AssertionResult test_center_ray() {
        auto ray = center_ray();
        rtx::Hit hit = sphere.ray_intersect(ray);
        return testing::AssertionResult(hit.is_valid());
    }

public:
    rtx::Camera camera;
    rtx::Sphere sphere;
};


TEST_F(CameraTest, move_forward) {
    camera.move(rtx::Direction::FORWARD);
    glm::vec3 expected {START_POS.x, START_POS.y,
                        START_POS.z - rtx::Camera::MOVE_SPEED};
    ASSERT_EQ(expected, camera.position());
}

TEST_F(CameraTest, move_backward) {
    camera.move(rtx::Direction::BACKWARD);
    glm::vec3 expected {START_POS.x, START_POS.y,
                        START_POS.z + rtx::Camera::MOVE_SPEED};
    ASSERT_EQ(expected, camera.position());
}

TEST_F(CameraTest, move_left) {
    camera.move(rtx::Direction::LEFT);
    glm::vec3 expected {START_POS.x - rtx::Camera::MOVE_SPEED,
                        START_POS.y, START_POS.z};
    ASSERT_EQ(expected, camera.position());
}

TEST_F(CameraTest, move_right) {
    camera.move(rtx::Direction::RIGHT);
    glm::vec3 expected {START_POS.x + rtx::Camera::MOVE_SPEED,
                        START_POS.y, START_POS.z};
    ASSERT_EQ(expected, camera.position());
}

TEST_F(CameraTest, rotate_left) {
    camera.move(rtx::Direction::ROTATE_LEFT);
}


TEST_F(CameraTest, emit_ray_move_left) {
    ASSERT_TRUE(test_center_ray());

    while (camera.position().x > -10.0) { camera.move(rtx::Direction::LEFT); }
    ASSERT_FALSE(test_center_ray());
}

TEST_F(CameraTest, emit_ray_move_right) {
    ASSERT_TRUE(test_center_ray());

    while (camera.position().x < 10.0) { camera.move(rtx::Direction::RIGHT); }
    ASSERT_FALSE(test_center_ray());
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
