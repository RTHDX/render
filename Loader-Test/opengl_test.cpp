#include <gtest/gtest.h>

#include <Loader/opengl_converter.hpp>


TEST(Loader, load_stone) {
    auto items = loader::Converter().read("./stone_1.obj");
    EXPECT_EQ(items.size(), 1);
    const auto& stone = items[0];
    auto& normal_1 = stone[0].normal;
    auto& normal_2 = stone[1].normal;
    auto& normal_3 = stone[2].normal;
    EXPECT_EQ(normal_1, normal_2);
    EXPECT_EQ(normal_2, normal_3);

    const size_t last   = stone.size() - 1;
    auto& normal_last_1 = stone[last].normal;
    auto& normal_last_2 = stone[last - 1].normal;
    auto& normal_last_3 = stone[last - 2].normal;

    EXPECT_EQ(normal_last_1, normal_last_2);
    EXPECT_EQ(normal_last_2, normal_last_3);
}
