#include <iostream>

#include <gtest/gtest.h>
#include <Render/Animation.hpp>

TEST(Animation, test_update) {
    render::Animation animation(0, 3);
    ASSERT_EQ(animation.current_index(), 0);
    animation.update();
    ASSERT_EQ(animation.current_index(), 1);
    animation.update();
    ASSERT_EQ(animation.current_index(), 2);
    animation.update();
    ASSERT_EQ(animation.current_index(), 0);
}

TEST(Animation, test_empty) {
    render::Animation animation;
    ASSERT_EQ(animation.current_index(), 0);
    animation.update();
    ASSERT_EQ(animation.current_index(), 0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
