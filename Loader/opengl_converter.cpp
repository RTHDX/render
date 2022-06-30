#include "opengl_converter.hpp"

namespace loader {

std::vector<opengl::Item> Converter::read(const std::string path) {
    auto meshes = loader.read(path);
    std::vector<opengl::Item> out(meshes.size());
    for (size_t i = 0; i < out.size(); ++i) {
        out[i] = convert(meshes[i]);
    }
    return out;
}

opengl::Item Converter::convert(const Mesh& mesh) {
    constexpr size_t stride = 3;

    std::vector<opengl::VertexData> vertex_data(mesh.vertices.size());
    for (size_t i = 0; i < mesh.vertices.size(); ++i) {
        vertex_data[i].position = mesh.vertices[i];
    }

    for (size_t i = 0; i < mesh.normals.size(); ++i) {
        const auto& normal = mesh.normals[i];
        vertex_data[i * 3].normal     = normal;
        vertex_data[i * 3 + 1].normal = normal;
        vertex_data[i * 3 + 2].normal = normal;
    }

    return {std::move(vertex_data)};
}

}
