#pragma once

#include <glad/glad.h>

#include "texture.hpp"
#include "comands.hpp"


namespace opengl {

struct framebuffer_data_t final {
    GLuint fbo;
    GLenum attachment_point; // GL_COLOR_ATTACHMENT0 ... GL_COLOR_ATTACHMENT31
    TextureData texture;
    GLenum target {GL_FRAMEBUFFER};

public:
    static framebuffer_data_t create(int width, int height);

    GLenum status() const;
    void free();
};


struct fbuff_ctx_guard_t final {
public:
    fbuff_ctx_guard_t(const fbuff_render_ctx_t& ctx);
    fbuff_ctx_guard_t(const fbuff_ctx_guard_t&)              = delete;
    fbuff_ctx_guard_t& operator = (const fbuff_ctx_guard_t&) = delete;
    fbuff_ctx_guard_t(fbuff_ctx_guard_t&&)                   = delete;
    fbuff_ctx_guard_t& operator = (fbuff_ctx_guard_t&&)      = delete;
    ~fbuff_ctx_guard_t();

private:
    fbuff_render_ctx_t ctx_;
};

}
