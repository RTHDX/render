#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "OpenGL/texture.hpp"
#include "OpenGL/opengl_vertex_input.hpp"
#include "Animation.hpp"

namespace render {

class Item_2D {
    using vertex_t = opengl::vec3pos_vec2tex_t;
    using vertices_t = std::vector<vertex_t>;
    using indices_t = std::vector<uint32_t>;

public:
    Item_2D(glm::mat4&& model,
            Animation&& animation,
            vertices_t&& box,
            indices_t&& indices;
            const opengl::TextureArray& atlas);
    ~Item_2D();

    void init();
    void update();
    void render(GLuint program);

private:
    glm::mat4 _model;
    Animation _animation;
    vertices_t _box;
    indices_t _indices;
    const opengl::TextureArray& _atlas;
    GLuint _vao;
    GLuint _ebo;
    GLuint _vbo;
};

}
