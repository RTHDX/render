#include <fstream>
#include <iostream>

#include "map_reader.hpp"


namespace map::reader {
using buff_iterator_t = std::istreambuf_iterator<char>;

Reader::Reader(const std::filesystem::path& path)
    : _path(path)
{}

Reader::Reader(std::filesystem::path&& path)
    : _path(std::move(path))
{}

bool Reader::read() {
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

void Reader::convert(std::string&& content) {
    std::cout << content << std::endl;
}

}
