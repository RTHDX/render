#pragma once

#include <vector>
#include <filesystem>
#include <glad/glad.h>

#include "comands.hpp"


namespace opengl {
using stencil_idx_t = GLubyte;


struct RenderData final {
    GLuint program;
    GLuint vao;
    std::vector<GLuint> vertex_buffers;
    GLuint ebo;
    stencil_idx_t stencil_ref;
    size_t ebo_count;

public:
    template<typename vertex_input_f>
    static RenderData create(const std::filesystem::path& vertex_shader,
                             const std::filesystem::path& fragment_shader,
                             const std::vector<vertex_input_f> vertex_in,
                             const std::vector<GLuint> elements_in) {
        RenderData self;
        self.program = opengl::create_program(vertex_shader, fragment_shader);
        self.vao = opengl::gen_vertex_array();
        self.ebo = opengl::gen_element_buffer();
        self.vertex_buffers = vertex_input_f::gen_buffers(
            self.vao, vertex_in,
            self.ebo, elements_in
        );
        self.ebo_count = elements_in.size();
        return self;
    }

    DrawElementsCommand draw_elements() const {
        return {
            .vao = vao,
            .count = ebo_count
        };
    }

    void free();
};

}
