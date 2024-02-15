#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>

#include "opengl_render_data.hpp"
#include "comands.hpp"


namespace opengl {

struct instant_render_data_t final {
    using path_t = std::filesystem::path;

    render_data_t impl;
    std::unordered_map<std::string, GLuint> buffers;
    GLsizei instance_count;

public:
    template <typename vertex_input_f>
    static instant_render_data_t create(const path_t& v_shader,
                                        const path_t& f_shader,
                                        const std::vector<vertex_input_f>& in,
                                        const std::vector<GLuint>& e_in) {
        render_data_t self_impl = render_data_t::create<vertex_input_f>(
            v_shader, f_shader, in, e_in
        );
        instant_render_data_t self;
        self.impl = std::move(self_impl);
        return self;
    }

    draw_elements_instanced_t draw_elements() const {
        return {
            .vao           = impl.vao,
            .count         = GLsizei(impl.ebo_count),
            .instancecount = instance_count
        };
    }

    draw_elements_instanced_fbuff_t
    draw_elements(const fbuff_render_ctx_t& ctx) const {
        return {
            .draw_cmd = draw_elements(),
            .fb_ctx   = ctx
        };
    }

    void free();
};

}
