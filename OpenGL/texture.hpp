#pragma once

#include <string>
#include <filesystem>
#include <variant>

#include <glad/glad.h>

#include "image_data.hpp"


namespace opengl {

struct TextureData final {
    GLuint id = 0;
    GLenum target; // GL_TEXTURE_1D, GL_TEXTURE_2D ...
    GLint w, h;
    GLint format; // GL_DEPTH_COMPONENT GL_DEPTH_STENCIL GL_RED GL_RG GL_RGB GL_RGBA
    GLenum type; // GL_UNSIGNED_BYTE...
    GLenum wrap_s, wrap_t, min_filter, mag_filter;

    void bind_with_image(const ImageData& image);
    void free();
    bool is_valid() const;
};


struct TextureDataArray2D final {
    TextureData tex_data;
    size_t tile_count_w {0}, tile_count_h {0};

    GLsizei tile_w() const; // pixels
    GLsizei tile_h() const; // pixels
    GLsizei total_tiles() const;
    bool is_valid() const;
    GLenum internal_format() const;
    GLsizei tile_offset(int x, int y) const;

    void free();
};

using any_texture_t = std::variant<TextureData, TextureDataArray2D>;

void set_texture_meta(byte_t* raw_data, const TextureData& params);
void set_texture_2d_array_meta(byte_t* raw_data, const TextureDataArray2D&);


}
