#include "opengl_vertex_input.hpp"


namespace opengl {

vec3pos_vec3norm_t::vec3pos_vec3norm_t(glm::vec3&& p, glm::vec3&& n)
    : pos(std::move(p))
    , normal(std::move(n))
{}

vec3pos_vec3norm_t::commands_t vec3pos_vec3norm_t::commands() {
    return {
        {.index=0, .stride=3, .offset=(void*)offsetof(this_t, pos)},
        {.index=1, .stride=3, .offset=(void*)offsetof(this_t, normal)}
    };
}


vec3pos_vec3norm_vec2tex_t::vec3pos_vec3norm_vec2tex_t(glm::vec3&& p,
                                                       glm::vec3&& n,
                                                       glm::vec2&& t)
    : pos(std::move(p))
    , norm(std::move(n))
    , tex_pos(std::move(t))
{}

vec3pos_vec3norm_vec2tex_t::commands_t
vec3pos_vec3norm_vec2tex_t::commands() {
    return {
        {.index=0, .stride=3, .offset=(void*)offsetof(this_t, pos)},
        {.index=1, .stride=3, .offset=(void*)offsetof(this_t, norm)},
        {.index=2, .stride=2, .offset=(void*)offsetof(this_t, tex_pos)}
    };
}


vec3pos_vec2tex_t::vec3pos_vec2tex_t(glm::vec3&& p, glm::vec2&& t)
    : pos(std::move(p))
    , tex_pos(std::move(t))
{}

vec3pos_vec2tex_t::commands_t vec3pos_vec2tex_t::commands() {
    return {
        {.index=0, .stride=3, .offset=(void*)offsetof(this_t, pos)},
        {.index=1, .stride=2, .offset=(void*)offsetof(this_t, tex_pos)}
    };
}

}
