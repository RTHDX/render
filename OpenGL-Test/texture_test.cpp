#include <iostream>

#include <gtest/gtest.h>

#include <UI/ui.hpp>
#include <OpenGL/opengl_proc.hpp>
#include <OpenGL/opengl_functions.hpp>
#include <OpenGL/texture.hpp>

#include <boost/stacktrace.hpp>

#include "function_provider_mock.hpp"

static auto& impl() {
    return static_cast<Invoker_Mock&>(opengl::Provider::instance().impl());
}

class Texture_Test : public testing::Test {
public:
    void TearDown() override {
        testing::Mock::VerifyAndClearExpectations(&impl());
    }
};


TEST(Texture, test) {
    EXPECT_CALL(impl(), gen_textures(testing::_, testing::_))
        .Times(testing::Exactly(1));
    EXPECT_CALL(impl(), bind_texture(testing::_, testing::_))
        .Times(testing::Exactly(1));
    EXPECT_CALL(impl(), tex_parameter_i(testing::_, testing::_, testing::_))
        .Times(testing::Exactly(4));
    EXPECT_CALL(impl(), delete_textures(testing::_, testing::_))
        .Times(testing::Exactly(1));

    opengl::Texture texture("hr-level1_running.png");
    ASSERT_TRUE(texture.read());

}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    testing::InitGoogleMock(&argc, argv);
    opengl::Provider::instance(new Invoker_Mock);
    return RUN_ALL_TESTS();
}
