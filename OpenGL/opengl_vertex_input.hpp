#pragma once

#include <glm/glm.hpp>


namespace opengl {

struct vec3pos_vec3norm_t {
    glm::vec3 pos;
    glm::vec3 normal;
};

struct vec3pos_vec3norm_vec2tex_t {
    glm::vec3 pos;
    glm::vec3 norm;
    glm::vec2 tex_pos;
};

struct vec3pos_vec2tex_t {
    glm::vec3 pos;
    glm::vec2 tex_pos;
};

}
