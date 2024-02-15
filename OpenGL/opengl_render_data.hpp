#pragma once

#include <vector>
#include <filesystem>
#include <glad/glad.h>

#include "comands.hpp"


namespace opengl {
using stencil_idx_t = GLubyte;


struct render_data_t final {
    GLuint program;
    GLuint vao;
    std::vector<GLuint> vertex_buffers;
    GLuint ebo;
    stencil_idx_t stencil_ref;
    GLsizei ebo_count;

public:
    template<typename vertex_input_f>
    static render_data_t create(const std::filesystem::path& vertex_shader,
                                const std::filesystem::path& fragment_shader,
                                const std::vector<vertex_input_f> vertex_in,
                                const std::vector<GLuint> elements_in) {
        render_data_t self;
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

    draw_elements_command_t draw_elements() const {
        return {
            .vao = vao,
            .count = ebo_count
        };
    }

    void free();
};

}
