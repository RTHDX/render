#pragma once

#include <OpenGL/opengl_vertex_input.hpp>

#include "visitor.hpp"
#include "loader.hpp"

namespace loader {
using Vertices = std::vector<opengl::vec3pos_vec3norm_t>;

class Converter {
public:
    Converter() = default;
    ~Converter() = default;

    std::vector<Vertices> read(const std::string path);

    void visit(const Face& face);
    Vertices convert(const Mesh& mesh);

private:
    Loader loader;
};

}
