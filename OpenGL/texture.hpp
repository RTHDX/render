#pragma once

#include <glad/glad.h>

namespace opengl {
using byte_t = unsigned char;

struct Texture {
    int width = 0;
    int height = 0;
    int depth = 0;
    byte_t* buffer = nullptr;

    GLuint id = 0;

public:
    Texture(const char* filepath);
    ~Texture();

    size_t size() const { return width * height * depth; }
};

}
