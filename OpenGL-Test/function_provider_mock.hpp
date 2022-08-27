#pragma once

#include <gmock/gmock.h>
#include <OpenGL/opengl_functions.hpp>

class Invoker_Mock final : public opengl::IInvoker {
public:
    Invoker_Mock() = default;
    ~Invoker_Mock() override = default;

    MOCK_METHOD(void, tex_parameter_i, (GLenum, GLenum, GLint));
    MOCK_METHOD(void, tex_image_2d, (GLenum, GLint, GLint, GLsizei, GLsizei,
                                     GLint, GLenum, GLenum, const void*));
    MOCK_METHOD(void, generate_mipmap, (GLenum));
    MOCK_METHOD(void, gen_textures, (GLsizei, GLuint*));
    MOCK_METHOD(void, bind_texture, (GLenum, GLuint));
    MOCK_METHOD(void, delete_textures, (GLsizei, GLuint*));
};