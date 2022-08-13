#define STB_IMAGE_IMPLEMENTATION
#include "3rdParty/stb/stb_image.h"

#include "texture.hpp"

namespace opengl {

Texture::Texture(const char* filepath) {
    stbi_set_flip_vertically_on_load(true);
    buffer = stbi_load(filepath, &width, &height, &depth, 3);
}

Texture::~Texture() {
    stbi_image_free(buffer);
}

}
