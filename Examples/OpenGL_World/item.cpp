#include <glm/gtx/transform.hpp>

#include <OpenGL/opengl_proc.hpp>

#include "item.hpp"

namespace fs = std::filesystem;

Item3D::Item3D(const fs::path& vertex,
               const fs::path& fragment,
               const glm::vec4& color,
               bool is_selectable)
    : _program(opengl::create_program(vertex, fragment))
    , _color(color)
    , _is_selectable(is_selectable)
{}

Item3D::Item3D(ItemInputData&& data)
    : _program(opengl::create_program(data.vertex, data.fragment))
    , _color(data.color)
    , _selection_program(opengl::create_program(data.vertex, data.fragment))
    , _selection_color(data.selection_color)
    , _is_selectable(data.is_selectable)
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
            .count = _vertices[i].size(),
        });
    }
}

void Item3D::modify(glm::mat4&& modificator) {
    //_model = modificator;
    std::swap(_model, modificator);
}

bool Item3D::activate(int id) {
    if (id == _id) {
        _is_active = true;
    } else {
        _is_active = false;
    }
    return _is_active;
}

bool Item3D::activate() {
    if (_is_selectable) {
        _is_active = true;
    }
    return _is_active;
}

void Item3D::deactivate() {
    _is_active = false;
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

void pass_selection_shader_uniforms(GLuint program,
                                    SelectionShaderUniformData&& data,
                                    const Item3D& item) {
    opengl::use(program);
    opengl::set_mat4(program, "model", data.model);
    opengl::set_mat4(program, "view", data.view);
    opengl::set_mat4(program, "projection", data.projection);
    item.draw();
}


Scene::Scene(std::vector<Item3D>&& items, opengl::Light&& light,
             opengl::Camera&& camera)
    : _items(std::move(items))
    , _light(std::move(light))
    , _camera(std::move(camera))
{}

Scene::Scene(opengl::Light&& light, opengl::Camera&& camera)
    : _light(std::move(light))
    , _camera(std::move(camera))
{}

Scene::~Scene() {
    for (auto& item : _items) {
        item.finalyze();
    }
}


void Scene::draw() {
    int stencil_ref = 1;
    for (auto& item : _items) {
        SAFE_CALL(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
        SAFE_CALL(glStencilFunc(GL_ALWAYS, stencil_ref, 0xFF));
        SAFE_CALL(glStencilMask(0xFF));
        item.id(stencil_ref);
        ++stencil_ref;
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


bool Scene::activate_index(GLuint index) {
    bool is_activated = false;
    for (auto& item : _items) {
        if (item.id() == index) {
            is_activated = item.activate();
        } else {
            item.deactivate();
        }
    }
    return is_activated;
}

Item3D* Scene::active_item() {
    for (Item3D& item : _items) {
        if (item.is_active()) {
            return &item;
        }
    }
    return nullptr;
}

bool Scene::is_any_item_active() const {
    return std::any_of(_items.cbegin(), _items.cend(), [](const Item3D& item) {
        return item.is_active();
    });
}


void Scene::append(Item3D&& item) {
    _items.push_back(std::move(item));
}

void Scene::clear() {
    _items.clear();
}
