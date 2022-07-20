#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "opengl.hpp"


namespace opengl {

using Index = uint32_t;
using Indices = std::vector<Index>;
using Coordinates = std::vector<float>;


struct VertexData {
    static constexpr size_t STRIDE = 3;

    glm::vec3 position;
    glm::vec3 normal;
};

struct VertexTexData {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_pos;
};


class BItem {
protected:
    enum State {
        NONE = 0x0,
        VERTEX_ARRAY_GENERATED = 0x1,
        VERTEX_POS_GENERATED = 0x2,
        VERTEX_POS_BOUND = 0x4,
        VERTEX_POS_ATTRIB_SET = 0x8,
        VERTEX_NORMAL_GENERATED = 0x10,
        VERTEX_NORMAL_BOUND = 0x20,
        VERTEX_NORMAL_ATTRIB_SET = 0x30,
    };

public:
    virtual ~BItem();
    virtual void initialize();
    virtual void draw() = 0;

    const glm::mat4& model() const { return _model; }

protected:
    void bind_vao();
    void unbind_vao();

    void update_state(State new_state) {
        _current_state = State(_current_state | new_state);
    }

private:
    void set_up_vertex_array();

private:
    glm::mat4 _model;
    GLuint _vao = 0;
    State _current_state;
};

class Item {
    enum State {
        NONE = 0x0,
        VERTEX_ARRAY_GENERATED = 0x1,
        VERTEX_POS_GENERATED = 0x2,
        VERTEX_POS_BOUND = 0x4,
        VERTEX_POS_ATTRIB_SET = 0x8,
        VERTEX_NORMAL_GENERATED = 0x10,
        VERTEX_NORMAL_BOUND = 0x20,
        VERTEX_NORMAL_ATTRIB_SET = 0x30,
    };

public:
    Item() = default;
    Item(std::vector<VertexData>&& vertices);

    ~Item() = default;

    void initialize();
    void draw();

    const glm::mat4& model() const { return _model; }
    void model(const glm::mat4& val) { _model = val; }
    const std::vector<VertexData>& vertices() const { return _vertices; }

private:
    void set_up_vertex_array();
    void set_up_vertex_buffer();

    void bind_vertex_array();
    void unbind_vertex_array();

    void update_state(State new_state) {
        _current_state = State(_current_state | new_state);
    }

private:
    std::vector<VertexData> _vertices;

    glm::mat4 _model;

    GLuint _vao = 0, _vbo_coordinates = 0, _vbo_normals = 0;

    State _current_state = NONE;
};

class TItem : public BItem {
public:
    TItem() = default;
    TItem(std::vector<VertexTexData>&& vertices);

    void initialize() override;
    void draw() override;

private:
    void set_up_vertex_buffers();

private:
    GLuint _tex_coords;
    std::vector<VertexTexData> _vertices;

    GLuint _vbo_coordinates = 0, _vbo_normals = 0, _tex_coordinates = 0;
};

}
