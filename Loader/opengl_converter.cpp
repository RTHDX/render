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

    std::vector<opengl::VertexData> vertex_data(mesh.faces.size() * stride);
    for (size_t i = 0; i < mesh.faces.size(); ++i) {
        const Face& face = mesh.faces[i];

        vertex_data[i * stride].position = mesh.vertices[face.indices[0]];
        vertex_data[i * stride + 1].position = mesh.vertices[face.indices[1]];
        vertex_data[i * stride + 2].position = mesh.vertices[face.indices[2]];

        vertex_data[i * stride].normal = mesh.normals[face.normal_index];
        vertex_data[i * stride + 1].normal = mesh.normals[face.normal_index];
        vertex_data[i * stride + 2].normal = mesh.normals[face.normal_index];
    }

    return {std::move(vertex_data)};
}

}
