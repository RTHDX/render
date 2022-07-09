#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>

#include "mesh.hpp"

struct aiMesh;
struct aiFace;
struct aiMaterial;
namespace loader {

class Loader {
public:
    Loader() = default;

    std::vector<Mesh> read(const std::string& path);

private:
    Mesh process_mesh(const aiMesh* mesh, const aiMaterial* material) const;
    Face process_face(const aiFace& face, Mesh* parent,
                      size_t normal_index) const;
    glm::vec3 process_vector(const aiVector3D& vector, bool n = false) const;

private:
    Assimp::Importer _importer;
};

}
