#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>

#include "opengl_render_data.hpp"
#include "comands.hpp"


namespace opengl {

struct InstantRenderData final {
    using path_t = std::filesystem::path;

    RenderData impl;
    std::unordered_map<std::string, GLuint> buffers;
    GLsizei instance_count;

public:
    template <typename vertex_input_f>
    static InstantRenderData create(const path_t& v_shader,
                                    const path_t& f_shader,
                                    const std::vector<vertex_input_f>& in,
                                    const std::vector<GLuint>& e_in) {
        RenderData self_impl = RenderData::create<vertex_input_f>(
            v_shader, f_shader, in, e_in
        );
        InstantRenderData self;
        self.impl = std::move(self_impl);
        return self;
    }

    DrawElementInstanced draw_elements() const {
        return {
            .vao           = impl.vao,
            .count         = GLsizei(impl.ebo_count),
            .instancecount = instance_count
        };
    }

    DrawElementsInstancedFramebuffer
    draw_elements(const fbuff_render_ctx_t& ctx) const {
        return {
            .draw_cmd = draw_elements(),
            .fb_ctx   = ctx
        };
    }

    void free();
};

}
