#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "visitor.hpp"


namespace loader {


struct Mesh;
struct Face : public Visitable {
    std::vector<size_t> indices;
    size_t normal_index;
    Mesh* mesh = nullptr;

public:
    Face() = default;
    Face(std::vector<size_t>&& indices, size_t n_i, Mesh* mesh);

    void accept(Visitor& visitor) {}
};


struct Mesh : public Visitable {
    std::vector<glm::vec3> vertices, normals;
    std::vector<Face> faces;

public:
    Mesh() = default;
    Mesh(std::vector<glm::vec3>&& vertices,
         std::vector<glm::vec3>&& normals,
         std::vector<Face>&& faces);

    void accept(Visitor& visitor) {}
};

}
