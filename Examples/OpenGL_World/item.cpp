#include <OpenGL/opengl_proc.hpp>

#include "item.hpp"


Item3D::~Item3D() {
    for (opengl::buffers_t& v_data : _vertex_input) {
        opengl::free_vertex_buffers(v_data);
    }
    opengl::free_vertex_array(_vao);
}

void Item3D::open(const std::string& path) {
    _vertices = loader::Converter().read(path);
    _vao = opengl::gen_vertex_array();
    for (const auto& vertex_data : _vertices) {
        _vertex_input.push_back(std::move(
            loader::Vertices::value_type::gen_buffers(_vao, vertex_data)
        ));
    }
}

void Item3D::draw() const {
    for (size_t i = 0; i < _vertices.size(); ++i) {
        opengl::draw(opengl::DrawArrayCommand{
            .vao = _vao,
            .count = _vertices[i].size()
        });
    }
}

void Item3D::modify(glm::mat4&& modificator) {
    _model = _model * modificator;
}


void pass_shader_uniforms(GLuint program, ShaderUniformData&& data,
                          const Item3D& item) {
    opengl::use(program);
    opengl::set_vec4(program, "color", data.color);
    opengl::set_vec3(program, "light_position", data.light_position);
    opengl::set_vec4(program, "light_color", data.light_color);
    opengl::set_mat4(program, "view", data.view);
    opengl::set_mat4(program, "projection", data.projection);
    opengl::set_mat4(program, "model", data.model);
    item.draw();
}
