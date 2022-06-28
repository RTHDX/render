#include "mesh.hpp"


namespace loader {

Mesh::Mesh(std::vector<glm::vec3>&& vertices,
           std::vector<glm::vec3>&& normals,
           std::vector<Face>&& faces)
    : Visitable()
    , vertices(std::move(vertices))
    , normals(std::move(normals))
    , faces(std::move(faces))
{}

Face::Face(std::vector<size_t>&& indices, size_t n_i, Mesh* mesh)
    : Visitable()
    , indices(std::move(indices))
    , normal_index(n_i)
    , mesh(mesh)
{}

}
