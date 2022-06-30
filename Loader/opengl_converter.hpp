#pragma once

#include <OpenGL/item.hpp>

#include "visitor.hpp"
#include "loader.hpp"

namespace loader {

class Converter {
public:
    Converter() = default;
    ~Converter() = default;

    std::vector<opengl::Item> read(const std::string path);

    void visit(const Face& face);
    opengl::Item convert(const Mesh& mesh);

private:
    opengl::Coordinates convert_vertices(const Mesh& mesh) const;
    opengl::Coordinates convert_normals(const Mesh& mesh) const;

private:
    Loader loader;
};

}
