#pragma once

#include <string>

#include <glad/glad.h>

namespace opengl {
using byte_t = unsigned char;

struct Texture {
    static constexpr GLenum TARGET = GL_TEXTURE_2D;

    int width = 0;
    int height = 0;
    int depth = 0;
    std::string path;
    byte_t* buffer = nullptr;

    GLuint id = 0;
    GLenum wrap_s = GL_NEAREST;
    GLenum wrap_t = GL_LINEAR;
    GLenum min_filter = GL_LINEAR_MIPMAP_LINEAR;
    GLenum mag_filter = GL_LINEAR;

public:
    static uint32_t channels();

    Texture(const char* filepath);
    ~Texture();

    bool read();
    void bind();
    void activate();
};

struct TextureArray {
    static constexpr GLenum TARGET = GL_TEXTURE_2D_ARRAY;

    int width = 0;
    int height = 0;
    int depth = 0;
    std::string path;
    uint32_t tiles_count_w;
    uint32_t tiles_count_h;
    byte_t* buffer = nullptr;
    bool is_read = false;

    GLuint id = 0;
    GLenum wrap_s = GL_CLAMP_TO_EDGE;
    GLenum wrap_t = GL_CLAMP_TO_EDGE;
    GLenum wrap_r = GL_CLAMP_TO_EDGE;
    GLenum min_filter = GL_LINEAR;
    GLenum mag_filter = GL_LINEAR;
    GLuint tex_base_lvl = 0;
    GLuint tex_max_lvl = 1;

public:
    static int read_mode();

    TextureArray(const char* filepath, uint32_t tcw, uint32_t tch);
    ~TextureArray();

    bool read();
    void bind();
    void activate();

    uint32_t tile_width() const {
        return buffer == nullptr ? 0 : width / tiles_count_w;
    }

    uint32_t tile_height() const {
        return buffer == nullptr ? 0 : height / tiles_count_h;
    }

    uint32_t total_tiles() const {
        return buffer == nullptr ? 0 : tiles_count_h * tiles_count_w;
    }
};

}
