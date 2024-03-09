#include <exception>
#include <iostream>
#include <iomanip>

#define STB_IMAGE_IMPLEMENTATION
#include "3rdParty/stb/stb_image.h"
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "texture.hpp"


namespace opengl {

ImageData ImageData::create(int w,
                            int h,
                            const std::vector<glm::u8vec4>& pixels) {
    if (pixels.size() / w != h) {
        throw std::runtime_error("Incorrect pixels.size()");
    }

    ImageData image;
    image.w = w;
    image.h = h;
    image.mode = ColorMode::RGBA;
    if (image.size() == 0) { return image; }
    image.data = new byte_t[image.size()];

    size_t p_i = 0;
    for (size_t i = 0; i < image.size();) {
        const auto& pixel = pixels[p_i];
        image.data[i++] = pixel.r;
        image.data[i++] = pixel.g;
        image.data[i++] = pixel.b;
        image.data[i++] = pixel.a;
        ++p_i;
    }

    return image;
}

ImageData ImageData::create(int w,
                            int h,
                            const std::vector<glm::u8vec3>& pixels) {
    if (pixels.size() / w != h) {
        throw std::runtime_error("Incorrect pixels.size()");
    }

    ImageData image;
    image.w = w;
    image.h = h;
    image.mode = ColorMode::RGB;
    if (image.size() == 0) { return image; }
    image.data = new byte_t[image.size()];
    std::copy(
        reinterpret_cast<const byte_t*>(pixels.data()),
        reinterpret_cast<const byte_t*>(pixels.data() + w * h),
        image.data
    );
    return image;
}

ImageData ImageData::create(int w,
                            int h,
                            glm::u8vec4 filler) {
    if (w <= 0 || h <= 0) {
        throw std::runtime_error("Incorrect format WxH");
    }

    ImageData image;
    image.w = w;
    image.h = h;
    image.mode = ColorMode::RGBA;
    image.data = new byte_t[image.size()];

    for (size_t i = 0; i < image.size();) {
        image.data[i++] = filler.r;
        image.data[i++] = filler.g;
        image.data[i++] = filler.b;
        image.data[i++] = filler.a;
    }

    return image;
}

ImageData ImageData::create(int w,
                            int h,
                            glm::u8vec3 filler) {
    if (w <= 0 || h <= 0) {
        throw std::runtime_error("Incorrect format WxH");
    }

    ImageData image;
    image.w = w;
    image.h = h;
    image.mode = ColorMode::RGB;
    image.data = new byte_t[image.size()];

    for (size_t i = 0; i < image.size();) {
        image.data[i++] = filler.r;
        image.data[i++] = filler.g;
        image.data[i++] = filler.b;
    }

    return image;
}

ImageData ImageData::read(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error(
            std::format("No image found {}", path.string())
        );
    }
    ImageData out;
    std::string str_path = path.string();
    int depth = 0;
    out.data = stbi_load(str_path.c_str(), &out.w, &out.h, &depth,
                         STBI_rgb_alpha);
    out.mode = ColorMode::RGBA;
    return out;
}

bool ImageData::write(std::filesystem::path path, const ImageData& data) {
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
    : w(other.w)
    , h(other.h)
    , mode(other.mode)
{
    if (other.data && other.size() > 0) {
        data = new byte_t[other.size()];
        std::copy(other.data, other.data + other.size(), data);
    }
}

ImageData& ImageData::operator=(const ImageData& other) {
    if (this != &other) {
        delete[] data;
        data = nullptr;
        w = other.w;
        h = other.h;
        mode = other.mode;
        if (other.data && other.size() > 0) {
            data = new byte_t[other.size()];
            std::copy(other.data, other.data + other.size(), data);
        }
    }
    return *this;
}

ImageData::ImageData(ImageData&& other) noexcept
    : data(other.data)
    , w(other.w)
    , h(other.h)
    , mode(other.mode)
{
    other.data = nullptr;
    other.w = -1;
    other.h = -1;
    other.mode = ColorMode::UNDEF;
}

ImageData& ImageData::operator=(ImageData&& other) noexcept {
    if (this != &other) {
        delete[] data;
        data = other.data;
        w = other.w;
        h = other.h;
        mode = other.mode;

        other.data = nullptr;
        other.w = -1;
        other.h = -1;
        other.mode = ColorMode::UNDEF;
    }

    return *this;
}

ImageData::~ImageData() {
    delete[] data;
}

bool ImageData::is_valid() const {
    return data != nullptr;
}

int ImageData::size() const {
    if (mode == ColorMode::UNDEF) { return 0; }
    return w * h * (mode == ColorMode::RGB ? 3 : 4);
}

void ImageData::dump() const {
    size_t d = size_t(mode);
    for (size_t i = 0; i < size(); ++i) {
        std::cout << std::setw(5) << int(data[i]);
        if ((i + 1) % d != 0) {
            std::cout << ", ";
        }
        if ((i + 1) % d == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

}
