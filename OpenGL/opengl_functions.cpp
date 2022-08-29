#include <iostream>
#include <boost/stacktrace.hpp>

#include "opengl_functions.hpp"

namespace opengl {

static void on_gl_error(GLenum error_code, const char* call, const char* file,
                        int line) {
    if (error_code == GL_NO_ERROR) return;

    std::cerr << call << " -> ";
    switch (error_code) {
    case GL_INVALID_ENUM:
        std::cerr << "INVALID_ENUM";
        break;
    case GL_INVALID_VALUE:
        std::cerr << "INVALID_VALUE";
        break;
    case GL_INVALID_OPERATION:
        std::cerr << "INVALID_OPERATION";
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        std::cerr << "INVALID_FRAMEBUFFER_OPERATION";
        break;
    case GL_OUT_OF_MEMORY:
        std::cerr << "OUT_OF_MEMORY";
        break;
    case GL_STACK_UNDERFLOW:
        std::cerr << "STACK_UNDERFLOW";
        break;
    case GL_STACK_OVERFLOW:
        std::cerr << "STACK_OVERFLOW";
        break;
    }
    std::cerr << std::endl << call << std::endl;
}
#define SAFE_CALL(gl_call)\
    gl_call;\
    opengl::on_gl_error(glGetError(), #gl_call, __FILE__, __LINE__);


void Invoker::tex_parameter_i(GLenum target, GLenum pname, GLint param) {
    SAFE_CALL(glTexParameteri(target, pname, param));
}

void Invoker::tex_image_2d(GLenum target, GLint level, GLint int_format,
                           GLsizei w, GLsizei h, GLint border, GLenum format,
                           GLenum type, const void* data) {
    SAFE_CALL(glTexImage2D(target, level, int_format, w, h, border, format,
                           type, data));
}

void Invoker::generate_mipmap(GLenum target) {
    SAFE_CALL(glGenerateMipmap(target));
}

void Invoker::gen_textures(GLsizei n, GLuint* textures) {
    SAFE_CALL(glGenTextures(n, textures));
}

void Invoker::bind_texture(GLenum target, GLuint tex) {
    SAFE_CALL(glBindTexture(target, tex));
}

void Invoker::activate_texture(GLenum target) {
    SAFE_CALL(glActiveTexture(target));
}

void Invoker::delete_textures(GLsizei n, GLuint* textures) {
    SAFE_CALL(glDeleteTextures(n, textures));
}

void Invoker::tex_storage_3d(GLenum target, GLsizei levels, GLenum iformat,
                             GLsizei w, GLsizei h, GLsizei depth) {
    SAFE_CALL(glTexStorage3D(target, levels, iformat, w, h, depth));
}

void Invoker::tex_subimage_3d(GLenum target, GLint level, GLint xoffset,
                              GLint yoffset, GLint zoffset, GLsizei width,
                              GLsizei height, GLsizei depth, GLenum format,
                              GLenum type, const void* pixels) {
    SAFE_CALL(glTexSubImage3D(target, level, xoffset, yoffset, zoffset,
                              width, height, depth, format, type, pixels));
}

void Invoker::tex_image_3d(GLenum target, GLint level, GLint internalformat,
                           GLsizei width, GLsizei height, GLsizei depth,
                           GLint border, GLenum format, GLenum type,
                           const void* data) {
    SAFE_CALL(glTexImage3D(target, level, internalformat, width, height,
                           depth, border, format, type, data));
}

Provider& Provider::instance(IInvoker* impl) {
    static Provider self(impl == nullptr ? new Invoker : impl);
    return self;
}

Provider::Provider(IInvoker* impl)
    : IInvoker()
    , _impl(impl)
{}

void Provider::tex_parameter_i(GLenum target, GLenum pname, GLint param) {
    _impl->tex_parameter_i(target, pname, param);
}

void Provider::tex_image_2d(GLenum target, GLint level, GLint int_format,
                            GLsizei w, GLsizei h, GLint border, GLenum format,
                            GLenum type, const void* data) {
    _impl->tex_image_2d(target, level, int_format, w, h, border, format,
                        type, data);
}

void Provider::generate_mipmap(GLenum target) {
    _impl->generate_mipmap(target);
}

void Provider::gen_textures(GLsizei n, GLuint* textures) {
    _impl->gen_textures(n, textures);
}

void Provider::bind_texture(GLenum t, GLuint tex) {
    _impl->bind_texture(t, tex);
}

void Provider::activate_texture(GLenum target) {
    _impl->activate_texture(target);
}

void Provider::delete_textures(GLsizei n, GLuint* textures) {
    _impl->delete_textures(n, textures);
}

void Provider::tex_storage_3d(GLenum target, GLsizei levels, GLenum iformat,
                             GLsizei w, GLsizei h, GLsizei depth) {
    _impl->tex_storage_3d(target, levels, iformat, w, h, depth);
}

void Provider::tex_subimage_3d(GLenum target, GLint level, GLint xoffset,
                               GLint yoffset, GLint zoffset, GLsizei width,
                               GLsizei height, GLsizei depth, GLenum format,
                               GLenum type, const void* pixels) {
    _impl->tex_subimage_3d(target, level, xoffset, yoffset, zoffset,
                           width, height, depth, format, type, pixels);
}

void Provider::tex_image_3d(GLenum target, GLint level, GLint internalformat,
                            GLsizei width, GLsizei height, GLsizei depth,
                            GLint border, GLenum format, GLenum type,
                            const void* data) {
    _impl->tex_image_3d(target, level, internalformat,
                        width, height, depth, border, format, type, data);
}

}
