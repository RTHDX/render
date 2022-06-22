#include <format>
#include <iostream>

#include <gtest/gtest.h>

#include <RTX/model.hpp>


TEST(Loader, read_iconsphere) {
    std::string path("icosphere.obj");
    rtx::model::Loader loader;
    auto meshes = loader.read(path);
    ASSERT_EQ(meshes.size(), 1);
    for (const auto& mesh : meshes) {
        std::cout << mesh.faces.size() << std::endl;
    }
}
