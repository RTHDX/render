#include <regex>
#include <fstream>
#include <iostream>
#include <algorithm>

#include <glm/glm.hpp>

#include "map_reader.hpp"


namespace map::reader {

Reader::Reader(const std::filesystem::path& path)
    : _path(path)
{}

Reader::Reader(std::filesystem::path&& path)
    : _path(std::move(path))
{}

bool Reader::read() {
    using buff_iterator_t = std::istreambuf_iterator<char>;

    std::wcout << "File: " << _path.c_str() << ", size: "
              << std::filesystem::file_size(_path) << std::endl;
    std::ifstream file (_path.c_str(), std::ios::in);
    if (!file.is_open()) {
        std::wcerr << "Could not read: " << _path.c_str() << std::endl;
        return false;
    }
    convert(std::move(std::string(
        (buff_iterator_t(file)), buff_iterator_t()
    )));
    return true;
}

bool Reader::verify() const {
    return _path.extension() == std::filesystem::path(".wrld");
}

static glm::ivec2 get_map_dims(const std::string& data);

void Reader::convert(std::string&& content) {
    const std::regex cell_pattern(R"(\d+)");
    const auto dims = get_map_dims(content);
    _height = dims.x;
    _width = dims.y;
    _map.resize(_height * _width);

    const auto begin = std::sregex_iterator(content.begin(), content.end(),
                                            cell_pattern);
    const auto end = std::sregex_iterator();

    assert(std::distance(begin, end) == _map.size());

    auto current = begin;
    for (ptrdiff_t i = 0; i < _map.size(); ++i) {
        auto match_object = *current;
        auto string = match_object.str();
        _map[i] = std::stoi(string);
        ++current;
    }

    assert(current == end);
}

static glm::ivec2 get_map_dims(const std::string& data) {
    const auto new_line_count = std::count(data.begin(),
                                           data.end(),
                                           '\n');
    const auto delim_total_count = std::count(data.begin(),
                                              data.end(),
                                              ' ');
    return {new_line_count, (delim_total_count / new_line_count) + 1};
}

}
