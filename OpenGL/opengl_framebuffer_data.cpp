#include "opengl_proc.hpp"
#include "opengl_framebuffer_data.hpp"


namespace opengl {

FramebufferData FramebufferData::create(int width, int height) {
    FramebufferData self {
        .fbo              = gen_framebuffer(),
        .attachment_point = GL_COLOR_ATTACHMENT0,
        .texture          = {
            .id         = gen_texture(),
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

GLenum FramebufferData::status() const {
    GLenum res = glCheckNamedFramebufferStatus(fbo, target);
    return res;
}

void FramebufferData::free() {
    if (!Context::instance().is_context_active()) { return; }

    if (fbo != 0) {
        SAFE_CALL(glDeleteFramebuffers(1, &fbo));
        fbo = 0;
    }
    texture.free();
}

}