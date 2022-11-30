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

private:
    void convert(std::string&& content);

public:
    map_t _map;
    std::filesystem::path _path;
};

}
