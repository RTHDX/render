#include "opengl_converter.hpp"

namespace loader {

std::vector<Vertices> Converter::read(const std::string& path) {
    auto meshes = loader.read(path);
    std::vector<Vertices> out(meshes.size());
    for (size_t i = 0; i < out.size(); ++i) {
        out[i] = convert(meshes[i]);
    }
    return out;
}

Vertices Converter::convert(const Mesh& mesh) {
    constexpr size_t stride = 3;

    Vertices out(mesh.faces.size() * stride);
    //std::vector<opengl::VertexData> vertex_data(mesh.faces.size() * stride);
    for (size_t i = 0; i < mesh.faces.size(); ++i) {
        const Face& face = mesh.faces[i];

        out[i * stride].pos     = mesh.vertices[face.indices[0]];
        out[i * stride + 1].pos = mesh.vertices[face.indices[1]];
        out[i * stride + 2].pos = mesh.vertices[face.indices[2]];

        out[i * stride].normal     = mesh.normals[face.normal_index];
        out[i * stride + 1].normal = mesh.normals[face.normal_index];
        out[i * stride + 2].normal = mesh.normals[face.normal_index];
    }

    return out;
}

}
