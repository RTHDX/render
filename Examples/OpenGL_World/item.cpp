#include <OpenGL/opengl_proc.hpp>

#include "item.hpp"

namespace fs = std::filesystem;

Item3D::Item3D(const fs::path& vertex,
               const fs::path& fragment,
               const glm::vec4& color)
    : _program(opengl::create_program(vertex, fragment))
    , _color(color)
{}

Item3D::~Item3D() {
    //for (opengl::buffers_t& v_data : _vertex_input) {
    //    opengl::free_vertex_buffers(v_data);
    //}
    //opengl::free_vertex_array(_vao);
}

void Item3D::finalyze() {
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
            .vao   = _vao,
            .count = _vertices[i].size()
        });
    }
}

void Item3D::modify(glm::mat4&& modificator) {
    _model = modificator;
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


Scene::Scene(std::vector<Item3D>&& items, opengl::Light&& light,
             opengl::Camera&& camera)
    : _items(std::move(items))
    , _light(std::move(light))
    , _camera(std::move(camera))
{}

Scene::~Scene() {
    for (auto& item : _items) {
        item.finalyze();
    }
}


void Scene::draw() {
    for (const auto& item : _items) {
        pass_shader_uniforms(item.program(), {
            .color = item.color(),
            .light_position = _light.position(),
            .light_color = _light.color(),
            .view = _camera.view(),
            .projection = _camera.projection(),
            .model = item.model()
        }, item);
    }
}
