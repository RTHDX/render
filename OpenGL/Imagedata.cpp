#define STB_IMAGE_IMPLEMENTATION
#include "3rdParty/stb/stb_image.h"
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "texture.hpp"


namespace opengl {


ImageData ImageData::read(const std::filesystem::path& path) {
    ImageData out;
    std::string str_path = path.string();
    out.data = stbi_load(str_path.c_str(), &out.w, &out.h, &out.d,
                         STBI_rgb_alpha);
    out.len  = out.w * out.h * out.d;
    out.mode = ColorMode::RGBA;
    return out;
}

bool ImageData::write(std::filesystem::path path,
                      const ImageData& data) {
    int ret = 0;
    std::string str_path;
    int w = data.w;
    int h = data.h;
    int m = int(data.mode);
    if (data.mode == ColorMode::RGB) {
        path.replace_extension(".jpg");
        str_path = path.string();
        ret = stbi_write_jpg(str_path.c_str(), w, h, (int)m, data.data,
                             w * (int)m);
    } else {
        path.replace_extension(".png");
        str_path = path.string();
        ret = stbi_write_png(str_path.c_str(), w, h, (int)m, data.data,
                             w * (int)m);
    }
    return ret != 0;
}

ImageData::ImageData(const ImageData& other)
    : len(other.len)
    , w(other.w)
    , h(other.h)
    , d(other.d)
    , mode(other.mode)
{
    if (other.data) {
        data = new byte_t[other.len];
        std::copy(other.data, other.data + other.len, data);
    }
}

ImageData& ImageData::operator=(const ImageData& other) {
    if (this != &other) {
        delete[] data;
        data = nullptr;
        len = other.len;
        w = other.w;
        h = other.h;
        d = other.d;
        mode = other.mode;
        if (other.data) {
            data = new byte_t[other.len];
            std::copy(other.data, other.data + other.len, data);
        }
    }
    return *this;
}

ImageData::ImageData(ImageData&& other) noexcept
    : data(other.data)
    , len(other.len)
    , w(other.w)
    , h(other.h)
    , d(other.d)
    , mode(other.mode)
{
    other.data = nullptr;
    other.len = -1;
    other.w = -1;
    other.h = -1;
    other.d = -1;
    other.mode = ColorMode::UNDEF;
}

ImageData& ImageData::operator=(ImageData&& other) noexcept {
    if (this != &other) {
        delete[] data;
        data = other.data;
        len = other.len;
        w = other.w;
        h = other.h;
        d = other.d;
        mode = other.mode;

        other.data = nullptr;
        other.len = -1;
        other.w = -1;
        other.h = -1;
        other.d = -1;
        other.mode = ColorMode::UNDEF;
    }

    return *this;
}

ImageData::~ImageData() {
    delete[] data;
}

bool ImageData::is_valid() const {
    return data != nullptr && len > 0;
}

}
