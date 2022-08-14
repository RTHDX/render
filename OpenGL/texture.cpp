#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "3rdParty/stb/stb_image.h"

#include "texture.hpp"

namespace opengl {

Texture::Texture(const char* filepath)
    : path(filepath) {
    stbi_set_flip_vertically_on_load(true);
}

bool Texture::read() {
    buffer = stbi_load(path.c_str(), &width, &height, &depth, 3);
    if (buffer == nullptr) {
        std::cerr << "Unalble to load texture " << path << std::endl;
        return false;
    }
    return true;
}

Texture::~Texture() {
    stbi_image_free(buffer);
}

}
