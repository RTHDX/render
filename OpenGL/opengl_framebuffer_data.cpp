#include "opengl_proc.hpp"
#include "opengl_framebuffer_data.hpp"


namespace opengl {

framebuffer_data_t framebuffer_data_t::create(int width, int height) {
    framebuffer_data_t self {
        .fbo              = gen_framebuffer(),
        .attachment_point = GL_COLOR_ATTACHMENT0,
        .texture          = {
            .id         = gen_texture(GL_TEXTURE_2D),
            .target     = GL_TEXTURE_2D,
            .w          = width,
            .h          = height,
            .format     = GL_RGBA,
            .type       = GL_UNSIGNED_BYTE,
            .wrap_s     = GL_CLAMP_TO_EDGE,
            .wrap_t     = GL_CLAMP_TO_EDGE,
            .min_filter = GL_LINEAR,
            .mag_filter = GL_LINEAR
        }
    };
    set_texture_meta(nullptr, self.texture);
    attach_texture(self, self.texture);
    return self;
}

GLenum framebuffer_data_t::status() const {
    GLenum res = glCheckNamedFramebufferStatus(fbo, target);
    return res;
}

void framebuffer_data_t::free() {
    if (!Context::instance().is_context_active()) { return; }

    if (fbo != 0) {
        SAFE_CALL(glDeleteFramebuffers(1, &fbo));
        fbo = 0;
    }
    texture.free();
}


void attach_texture(const framebuffer_data_t& fbuff, const texture_data_t& tex) {
    SAFE_CALL(glBindFramebuffer(fbuff.target, fbuff.fbo))
    SAFE_CALL(glFramebufferTexture2D(fbuff.target, fbuff.attachment_point,
                                     tex.target, tex.id, 0));
    SAFE_CALL(glBindFramebuffer(fbuff.target, 0));
}


fbuff_ctx_guard_t::fbuff_ctx_guard_t(const fbuff_render_ctx_t& ctx)
    : ctx_(ctx)
{
    SAFE_CALL(glBindFramebuffer(GL_FRAMEBUFFER, ctx.fbo));
    SAFE_CALL(glClearColor(
        ctx.background.r,
        ctx.background.g,
        ctx.background.b,
        ctx.background.a
    ));
    SAFE_CALL(glClear(ctx.clear_bits));
    SAFE_CALL(glViewport(ctx.viewport.x, ctx.viewport.y,
                         ctx.viewport.z, ctx.viewport.w));
}

fbuff_ctx_guard_t::~fbuff_ctx_guard_t() {
    SAFE_CALL(glViewport(ctx_.screen_viewport.x, ctx_.screen_viewport.y,
                         ctx_.screen_viewport.z, ctx_.screen_viewport.w));
    SAFE_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

}