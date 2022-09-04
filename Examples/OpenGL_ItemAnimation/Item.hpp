#pragma once

#include <map>

#include <glm/glm.hpp>

#include <OpenGL/texture.hpp>
#include <OpenGL/opengl_vertex_input.hpp>
#include <Render/Animation.hpp>


enum class Direction {
    NORTH,
    EAST_NORTH,
    EAST,
    EAST_SOUTH,
    SOUTH,
    SOUTH_WEST,
    WEST,
    WEST_NORTH
};

struct Item_2D {
    std::vector<opengl::vec3pos_vec2tex_t> input;
    std::vector<uint32_t> indices;
    glm::mat4 model;
    opengl::TextureArray atlas;
    std::map<Direction, render::Animation> animations;

    GLuint vao = 0;
    GLuint ebo = 0;
    GLuint vbo = 0;

public:
    void update();
    void render(GLuint program);

private:
    render::Animation& active_animation() const;
};
