#pragma once

#include <glm/glm.hpp>

#include "OpenGL/texture.hpp"
#include "Animation.hpp"

namespace render {

class Item_2D {
public:
    Item_2D();

    void update();

private:
    glm::mat4 _model;
    Animation _animation;
    opengl::TextureArray _texture;
};

}
