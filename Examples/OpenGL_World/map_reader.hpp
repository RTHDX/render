#pragma once

#include <inttypes.h>
#include <vector>
#include <filesystem>

namespace map::reader {

using cell_t = uint64_t;
using map_t = std::vector<cell_t>;


class Reader {
public:
    Reader(const std::filesystem::path& path);
    Reader(std::filesystem::path&& path);

    bool read();
    bool verify() const;

    const map_t& map() const { return _map; }
    uint32_t width() const { return _width; }
    uint32_t height() const { return _height; }

private:
    void convert(std::string&& content);

public:
    uint32_t _width = {0}, _height = {0};
    map_t _map;
    std::filesystem::path _path;
};

}
