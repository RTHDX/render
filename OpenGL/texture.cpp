#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "3rdParty/stb/stb_image.h"

#include "opengl_proc.hpp"
#include "texture.hpp"
#include "opengl_functions.hpp"

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
    Provider::instance().tex_parameter_i(TARGET, GL_TEXTURE_MIN_FILTER,
                                         min_filter);
    Provider::instance().tex_parameter_i(TARGET, GL_TEXTURE_MAG_FILTER,
                                         mag_filter);
    Provider::instance().tex_parameter_i(TARGET, GL_TEXTURE_WRAP_S, wrap_s);
    Provider::instance().tex_parameter_i(TARGET, GL_TEXTURE_WRAP_T, wrap_t);
    return true;
}

void Texture::bind() {
    SAFE_CALL(glTexImage2D(TARGET, 0, GL_RGBA, width, height, 0, GL_RGBA,
                           GL_UNSIGNED_BYTE, buffer));
    SAFE_CALL(glGenerateMipmap(GL_TEXTURE_2D));
}

void Texture::activate() {
    SAFE_CALL(glActiveTexture(GL_TEXTURE0));
}

Texture::~Texture() {
    stbi_image_free(buffer);
    SAFE_CALL(glDeleteTextures(1, &id));
}


int TextureArray::read_mode() {
    return STBI_rgb_alpha;
}

TextureArray::TextureArray(const char* filepath, uint32_t tcw, uint32_t tch)
    : path(filepath)
    , tiles_count_w(tcw)
    , tiles_count_h(tch)
{}

TextureArray::~TextureArray() {
    stbi_image_free(buffer);
}

bool TextureArray::read() {
    buffer = stbi_load(path.c_str(), &width, &height, &depth, read_mode());
    if (buffer == nullptr) {
        std::cerr << "Unalble to load texture " << path << std::endl;
        is_read = false;
        return false;
    }
    const GLsizei tile_w = width / tiles_count_w,
                  tile_h = height / tiles_count_h,
                  total_tiles = tiles_count_h * tiles_count_w;
    // create texture;
    SAFE_CALL(glGenTextures(1, &id));
    SAFE_CALL(glBindTexture(TARGET, id));
    // set texture parameters
    SAFE_CALL(glTexParameteri(TARGET, GL_TEXTURE_BASE_LEVEL, 0));
    SAFE_CALL(glTexParameteri(TARGET, GL_TEXTURE_MAX_LEVEL, 1));
    SAFE_CALL(glTexParameteri(TARGET, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    SAFE_CALL(glTexParameteri(TARGET, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    SAFE_CALL(glTexParameteri(TARGET, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    SAFE_CALL(glTexParameteri(TARGET, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    // set tiles sizes and amount
    SAFE_CALL(glTexStorage3D(TARGET, 1, GL_RGBA8, tile_w, tile_h,
                             total_tiles));
    // set image dims
    SAFE_CALL(glPixelStorei(GL_UNPACK_ROW_LENGTH, width));
    SAFE_CALL(glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, height));
    // cropp frames
    for (GLsizei i = 0; i < total_tiles; ++i) {
        int ix = i % tiles_count_h;
        int iy = i / tiles_count_w;
        int x = ix * tile_w;
        int y = iy * tile_h;
        SAFE_CALL(glTexSubImage3D(
            GL_TEXTURE_2D_ARRAY,
            0,
            0, 0, i,
            tile_w, tile_h, 1,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            buffer + ((y * width + x) * 4)
        ));
    }
    SAFE_CALL(glGenerateMipmap(TARGET));
    is_read = true;
    return true;
}

void TextureArray::bind() {
    SAFE_CALL(glBindTexture(TARGET, id));
}

}
