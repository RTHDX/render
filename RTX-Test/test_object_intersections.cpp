#include <filesystem>

#include <gtest/gtest.h>

#include <RTX/rtx.hpp>
#include <RTX/scene.hpp>


constexpr size_t TEST_WIDTH = 200;
constexpr size_t TEST_HEIGHT = 200;
constexpr rtx::Point START_POS{0.0, 0.0, 10.0};
constexpr rtx::Point START_TAR{0.0, 0.0, 0.0};


class TriangleTest : public testing::Test {
public:
    TriangleTest()
        : testing::Test()
        , camera(START_POS, START_TAR, glm::radians(60.0),
                 TEST_WIDTH, TEST_HEIGHT)
    {}

public:
    rtx::Camera camera;
};


TEST_F(TriangleTest, face) {
    rtx::Triangle triangle({0, 5.0, 0}, {-2.5, -2.5, 0}, {2.5, -2.5, 0},
                           {0, 0, 1.0}, rtx::Material());
    auto ray = camera.emit_ray(TEST_HEIGHT / 2, TEST_WIDTH / 2);
    auto hit = triangle.ray_intersect(ray);
    ASSERT_TRUE(hit.is_valid());

    ray = camera.emit_ray(0, 0);
    hit = triangle.ray_intersect(ray);
    ASSERT_FALSE(hit.is_valid());
}

TEST_F(TriangleTest, surface) {
    rtx::Triangle triangle({0, 5.0, 0}, {-2.5, -2.5, 0}, {2.5, -2.5, 0},
                           {0, 0, -1.0}, rtx::Material());
    auto ray = camera.emit_ray(TEST_HEIGHT / 2, TEST_WIDTH / 2);
    auto hit = triangle.ray_intersect(ray);
    ASSERT_FALSE(hit.is_valid());
}

auto current_dir() {
    std::wstring temp(std::filesystem::current_path().c_str());
    return std::string(temp.begin(), temp.end());
}


TEST(MeshTest, cube) {
    auto meshes = rtx::model::Loader()
        .read(std::format(R"({}\{})", current_dir(), "cube.obj"));
    std::cout << meshes.size() << std::endl;

    rtx::Scene<rtx::Mesh> scene;
}
