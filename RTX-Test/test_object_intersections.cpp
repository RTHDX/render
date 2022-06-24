#include <filesystem>

#include <gtest/gtest.h>

#include <RTX/rtx.hpp>
#include <RTX/scene.hpp>


constexpr size_t TEST_WIDTH = 200;
constexpr size_t TEST_HEIGHT = 200;
constexpr rtx::Point START_POS{0.0, 0.0, 2.0};
constexpr rtx::Point START_TAR{0.0, 0.0, 0.0};

auto create_camera(const rtx::Point& pos=START_POS) {
    return rtx::Camera(
        START_POS, START_TAR, glm::radians(90.0), TEST_WIDTH, TEST_HEIGHT
    );
}


class TriangleTest : public testing::Test {
public:
    TriangleTest()
        : testing::Test()
        , camera(std::move(create_camera()))
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

    ray = camera.emit_ray(TEST_HEIGHT - 1, TEST_WIDTH - 1);
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

TEST(Square, test) {
    std::vector<rtx::Triangle> faces {
        rtx::Triangle({-1, 1, 0}, {1, 1, 0}, {1, -1, 0},
                      {0, 0, 1}, rtx::Material()),
        rtx::Triangle({-1, 1, 0}, {-1, -1, 0}, {1, -1, 0},
                      {0, 0, 1}, rtx::Material())
    };
    rtx::Mesh square;
    square.faces = faces;
    auto camera = create_camera();
    auto ray = camera.emit_ray(TEST_HEIGHT / 2, TEST_WIDTH / 2);
    auto hit = square.ray_intersect(ray);
    ASSERT_TRUE(hit.is_valid());
}

/*
TEST(MeshTest, cube) {
    rtx::model::Loader loader;
    std::string path = std::format(R"({}\{})", current_dir(), "cube.obj");
    std::vector<rtx::Mesh> meshes = loader.read(path);
    rtx::Mesh cube = meshes[0];

    auto camera = create_camera({3.0, 3.0, 3.0});
    auto ray = camera.emit_ray(TEST_HEIGHT / 2, TEST_WIDTH / 2);
    auto hit = cube.ray_intersect(ray);
}
*/
