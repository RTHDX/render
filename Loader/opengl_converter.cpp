#include "opengl_converter.hpp"

namespace loader{

void Converter::read(const std::string path) {
    auto meshes = loader.read(path);
}

opengl::Coordinates Converter::convert_vertices(const Mesh& mesh) const {
    const size_t stride = 3;
    opengl::Coordinates vertices(mesh.vertices.size() * stride);

    for (size_t i = 0; i < mesh.vertices.size(); ++i) {
        vertices[stride * i] = mesh.vertices[i].x;
        vertices[stride * i + 1] = mesh.vertices[i].y;
        vertices[stride * i + 2] = mesh.vertices[i].z;
    }

    return vertices;
}

opengl::Coordinates Converter::convert_normals(const Mesh& mesh) const {
    const size_t stride = 3;
    opengl::Coordinates normals(mesh.faces.size() * stride);

    for (size_t i = 0; i < mesh.faces.size() * stride; ++i) {
        normals[stride * i] = mesh.vertices[i].x;
        normals[stride * i + 1] = mesh.vertices[i].y;
        normals[stride * i + 2] = mesh.vertices[i].z;
    }

    return normals;
}

opengl::Item Converter::convert(const Mesh& mesh) {
    constexpr size_t stride = 3;
    opengl::Coordinates vertices(mesh.vertices.size() * stride);

    for (size_t i = 0; i < mesh.vertices.size(); ++i) {
        vertices[stride * i]     = mesh.vertices[i].x;
        vertices[stride * i + 1] = mesh.vertices[i].y;
        vertices[stride * i + 2] = mesh.vertices[i].z;
    }

    opengl::Coordinates normals(mesh.normals.size() * stride);
    for (size_t i = 0; i < mesh.normals.size(); ++i) {
        size_t current = stride * i;
        normals[current]     = mesh.normals[i].x;
        normals[current + 1] = mesh.normals[i].y;
        normals[current + 2] = mesh.normals[i].z;
    }

    return {
        std::move(vertices),
        std::move(normals),
        std::move(opengl::AttribPointer(0, 3, 3))
    };
}

}
