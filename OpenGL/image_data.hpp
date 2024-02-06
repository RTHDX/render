#pragma once

#include <vector>
#include <filesystem>

#include <glm/glm.hpp>


namespace opengl {
using byte_t = unsigned char;

enum class ColorMode {
    UNDEF = 0,
    RGB = 3,
    RGBA
};

struct ImageData final {
    byte_t* data   {nullptr};
    int w          {-1},
        h          {-1};
    ColorMode mode {ColorMode::UNDEF};

public:
    static ImageData create(
        int w,
        int h,
        const std::vector<glm::u8vec4>& pixels
    );
    static ImageData create(
        int w,
        int h,
        const std::vector<glm::u8vec3>& pixels
    );
    static ImageData create(
        int w,
        int h,
        glm::u8vec4 filler
    );
    static ImageData create(
        int w,
        int h,
        glm::u8vec3 filler
    );

    static ImageData read(const std::filesystem::path& path);
    static bool write(std::filesystem::path path, const ImageData& d);

    ImageData() = default;
    ImageData(const ImageData& other);
    ImageData& operator=(const ImageData& other);
    ImageData(ImageData&& other) noexcept;
    ImageData& operator=(ImageData&& other) noexcept;
    ~ImageData();

    bool is_valid() const;
    int size() const;
    void dump() const;
};

}