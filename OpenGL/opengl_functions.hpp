#pragma once

#include <memory>
#include <glad/glad.h>

namespace opengl {

class IInvoker {
public:
    virtual ~IInvoker() = default;

    virtual void tex_parameter_i(GLenum target, GLenum pname, GLint param) = 0;
    virtual void tex_image_2d(GLenum target, GLint level, GLint int_format,
                              GLsizei w, GLsizei h, GLint border, GLenum format,
                              GLenum type, const void* data) = 0;
    virtual void generate_mipmap(GLenum target) = 0;
    virtual void gen_textures(GLsizei n, GLuint* textures) = 0;
    virtual void bind_texture(GLenum target, GLuint tex) = 0;
    virtual void activate_texture(GLenum target) = 0;
    virtual void delete_textures(GLsizei n, GLuint* textures) = 0;
    virtual void tex_storage_3d(GLenum target, GLsizei levels, GLenum iformat,
                                GLsizei w, GLsizei h, GLsizei depth) = 0;
    virtual void tex_subimage_3d(GLenum target, GLint level, GLint xoffset,
                                 GLint yoffset, GLint zoffset, GLsizei width,
                                 GLsizei height, GLsizei depth, GLenum format,
                                 GLenum type, const void* pixels) = 0;
    virtual void tex_image_3d(GLenum target, GLint level, GLint internalformat,
                              GLsizei width, GLsizei height, GLsizei depth,
                              GLint border, GLenum format, GLenum type,
                              const void* data) = 0;
};

class Invoker final : public IInvoker {
public:
    Invoker() = default;
    ~Invoker() override = default;

    void tex_parameter_i(GLenum target, GLenum pname, GLint param) override;
    void tex_image_2d(GLenum target, GLint level, GLint int_format,
                      GLsizei w, GLsizei h, GLint border, GLenum format,
                      GLenum type, const void* data) override;
    void generate_mipmap(GLenum target) override;
    void gen_textures(GLsizei n, GLuint* textures) override;
    void bind_texture(GLenum target, GLuint tex) override;
    void activate_texture(GLenum target) override;
    void delete_textures(GLsizei n, GLuint* textures) override;
    void tex_storage_3d(GLenum target, GLsizei levels, GLenum iformat,
                        GLsizei w, GLsizei h, GLsizei depth) override;
    void tex_image_3d(GLenum target, GLint level, GLint internalformat,
                      GLsizei width, GLsizei height, GLsizei depth,
                      GLint border, GLenum format, GLenum type,
                      const void* data) override;
    void tex_subimage_3d(GLenum target, GLint level, GLint xoffset,
                         GLint yoffset, GLint zoffset, GLsizei width,
                         GLsizei height, GLsizei depth, GLenum format,
                         GLenum type, const void* pixels) override;
};

class Provider final : public IInvoker {
public:
    static Provider& instance(IInvoker* impl=nullptr);

    void tex_parameter_i(GLenum target, GLenum pname, GLint param) override;
    void tex_image_2d(GLenum target, GLint level, GLint int_format,
                      GLsizei w, GLsizei h, GLint border, GLenum format,
                      GLenum type, const void* data) override;
    void generate_mipmap(GLenum target) override;
    void gen_textures(GLsizei n, GLuint* textures) override;
    void bind_texture(GLenum target, GLuint tex) override;
    void activate_texture(GLenum target) override;
    void delete_textures(GLsizei n, GLuint* textures) override;
    void tex_storage_3d(GLenum target, GLsizei levels, GLenum iformat,
                        GLsizei w, GLsizei h, GLsizei depth) override;
    void tex_image_3d(GLenum target, GLint level, GLint internalformat,
                      GLsizei width, GLsizei height, GLsizei depth,
                      GLint border, GLenum format, GLenum type,
                      const void* data) override;
    void tex_subimage_3d(GLenum target, GLint level, GLint xoffset,
                         GLint yoffset, GLint zoffset, GLsizei width,
                         GLsizei height, GLsizei depth, GLenum format,
                         GLenum type, const void* pixels) override;

    const IInvoker& impl() const { return *_impl; }
    IInvoker& impl() { return *_impl; }

private:
    Provider(IInvoker* impl);

private:
    std::unique_ptr<IInvoker> _impl;
};

inline Provider& function() { return Provider::instance(); }

}
