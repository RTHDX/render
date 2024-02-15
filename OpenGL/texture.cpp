#include <iostream>
#include <format>

#include "opengl_proc.hpp"
#include "texture.hpp"


namespace opengl {

void texture_data_t::bind_with_image(const ImageData& image) {
    set_texture_meta(image.data, *this);
}

void texture_data_t::free() {
    if (id != 0 && Context::instance().is_context_active()) {
        SAFE_CALL(glBindTexture(target, 0));
        SAFE_CALL(glDeleteTextures(1, &id));
        id = 0;
    }
}

bool texture_data_t::is_valid() const {
    return id != 0;
}


GLsizei texture_data_array_2d_t::tile_w() const {
    return tex_data.w / tile_count_w;
}

GLsizei texture_data_array_2d_t::tile_h() const {
    return tex_data.h / tile_count_h;
}

GLsizei texture_data_array_2d_t::total_tiles() const {
    return tile_count_h * tile_count_w;
}

bool texture_data_array_2d_t::is_valid() const {
    return tex_data.is_valid() && tile_count_h != 0 && tile_count_w != 0;
}

GLenum texture_data_array_2d_t::internal_format() const {
    GLenum format = tex_data.format;
    switch (format) {
    case GL_RGB:  return GL_RGB8;
    case GL_RGBA: return GL_RGBA8;
    default:
        std::cerr << "Invalid format " << format;
        return 0;
    }
}

GLsizei texture_data_array_2d_t::tile_offset(int x, int y) const {
    GLenum format = tex_data.format;
    GLsizei stride = 0;
    if (format == GL_RGB) {
        stride = 3;
    } else if (format == GL_RGBA) {
        stride = 4;
    }

    if (stride == 0) {
        std::cerr << "Sride is zero" << std::endl;
    }
    return (y * tex_data.w + x) * stride;
}

void texture_data_array_2d_t::free() {
    tex_data.free();
    tile_count_w = 0;
    tile_count_h = 0;
}

void texture_data_array_2d_t::bind_with_image(const ImageData& image) {
    set_texture_2d_array_meta(image.data, *this);
}


void set_texture_meta(byte_t* raw_data, const texture_data_t& params) {
    SAFE_CALL(glBindTexture(params.target, params.id));
    SAFE_CALL(glTexImage2D(params.target, 0, params.format, params.w, params.h,
                           0, params.format, params.type, raw_data));
    SAFE_CALL(glTexParameteri(params.target, GL_TEXTURE_WRAP_S, params.wrap_s));
    SAFE_CALL(glTexParameteri(params.target, GL_TEXTURE_WRAP_T, params.wrap_t));
    SAFE_CALL(glTexParameteri(params.target, GL_TEXTURE_MIN_FILTER,
        params.min_filter));
    SAFE_CALL(glTexParameteri(params.target, GL_TEXTURE_MAG_FILTER,
        params.mag_filter));
    SAFE_CALL(glBindTexture(params.target, 0));
}

void set_texture_2d_array_meta(byte_t* raw_data,
                               const texture_data_array_2d_t& data) {
    const GLenum t = data.tex_data.target;
    SAFE_CALL(glBindTexture(t, data.tex_data.id));

    SAFE_CALL(glTexParameteri(t, GL_TEXTURE_BASE_LEVEL, 0));
    SAFE_CALL(glTexParameteri(t, GL_TEXTURE_MAX_LEVEL, 1));
    SAFE_CALL(glTexParameteri(t, GL_TEXTURE_MAG_FILTER,
                              data.tex_data.mag_filter));
    SAFE_CALL(glTexParameteri(t, GL_TEXTURE_MIN_FILTER,
                              data.tex_data.min_filter));
    SAFE_CALL(glTexParameteri(t, GL_TEXTURE_WRAP_S, data.tex_data.wrap_s));
    SAFE_CALL(glTexParameteri(t, GL_TEXTURE_WRAP_T, data.tex_data.wrap_t));

    GLsizei tile_width = data.tile_w();
    GLsizei tile_height = data.tile_h();
    GLsizei total_tiles = data.total_tiles();
    SAFE_CALL(glTexStorage3D(
        t,
        1,
        data.internal_format(),
        tile_width,
        tile_height,
        total_tiles
    ));

    SAFE_CALL(glPixelStorei(GL_UNPACK_ROW_LENGTH, data.tex_data.w));
    SAFE_CALL(glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, data.tex_data.h));

    for (GLsizei i = 0; i < total_tiles; ++i) {
        int ix = i % data.tile_count_w;
        int iy = i / data.tile_count_w;
        int x = ix * tile_width;
        int y = iy * tile_height;
        byte_t* offset = raw_data + data.tile_offset(x, y);
        SAFE_CALL(glTexSubImage3D(
            t,
            0,
            0, 0, i,
            data.tile_w(), data.tile_h(), 1,
            data.tex_data.format,
            data.tex_data.type,
            offset
        ));
    }

    SAFE_CALL(glGenerateMipmap(t));
    SAFE_CALL(glBindTexture(t, 0));
}

}
