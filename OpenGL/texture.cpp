#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "3rdParty/stb/stb_image.h"

#include "opengl_proc.hpp"
#include "texture.hpp"

namespace opengl {

uint32_t Texture::channels() {
    return STBI_rgb_alpha;
}

Texture::Texture(const char* filepath)
    : path(filepath) {
    stbi_set_flip_vertically_on_load(true);
}

bool Texture::read() {
    buffer = stbi_load(path.c_str(), &width, &height, &depth, channels());
    if (buffer == nullptr) {
        std::cerr << "Unalble to load texture " << path << std::endl;
        return false;
    }
    id = gen_texture(TARGET);
    SAFE_CALL(glTexParameteri(TARGET, GL_TEXTURE_MIN_FILTER, min_filter));
    SAFE_CALL(glTexParameteri(TARGET, GL_TEXTURE_MAG_FILTER, mag_filter));
    SAFE_CALL(glTexParameteri(TARGET, GL_TEXTURE_WRAP_S, wrap_s));
    SAFE_CALL(glTexParameteri(TARGET, GL_TEXTURE_WRAP_T, wrap_t));
    return true;
}

void Texture::bind() {
    SAFE_CALL(glTexImage2D(TARGET, 0, GL_RGBA, width, height, 0, GL_RGBA,
                           GL_UNSIGNED_BYTE, buffer));
    SAFE_CALL(glGenerateMipmap(GL_TEXTURE_2D));
}

void Texture::activate() {
    //SAFE_CALL(glBindTexture(GL_TEXTURE_2D, id));
    SAFE_CALL(glActiveTexture(GL_TEXTURE0));
}

Texture::~Texture() {
    stbi_image_free(buffer);
    free_texture(id);
}


TextureArray::TextureArray(const char* filepath, uint32_t tcw, uint32_t tch)
    : path(filepath)
    , tiles_count_w(tcw)
    , tiles_count_h(tch)
{
    stbi_set_flip_vertically_on_load(true);
}

TextureArray::~TextureArray() {
    stbi_image_free(buffer);
}

bool TextureArray::read() {
    buffer = stbi_load(path.c_str(), &width, &height, &depth, STBI_rgb_alpha);
    if (buffer == nullptr) {
        std::cerr << "Unalble to load texture " << path << std::endl;
        return false;
    }
    id = gen_texture(TARGET);
    SAFE_CALL(glTexParameteri(TARGET, GL_TEXTURE_MIN_FILTER, min_filter));
    SAFE_CALL(glTexParameteri(TARGET, GL_TEXTURE_MAG_FILTER, mag_filter));
    SAFE_CALL(glTexParameteri(TARGET, GL_TEXTURE_WRAP_S, wrap_s));
    SAFE_CALL(glTexParameteri(TARGET, GL_TEXTURE_WRAP_T, wrap_t));
    SAFE_CALL(glTexParameteri(TARGET, GL_TEXTURE_WRAP_R, wrap_r));
    return true;
}

}
