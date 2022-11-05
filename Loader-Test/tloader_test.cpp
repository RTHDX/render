#include <gtest/gtest.h>

#include <Loader/tloader.hpp>


TEST(Loader, load_scene) {
    loader::TLoader loader;
    ASSERT_TRUE(loader.read(std::filesystem::path("cube.gltf")));
}
