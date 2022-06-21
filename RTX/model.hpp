#pragma once

#include <assimp/Importer.hpp>

#include "rtx_internal.hpp"


struct aiMesh;
struct aiFace;
namespace rtx::model {

class Loader {
public:
    Loader() = default;

    std::vector<rtx::Mesh> read(const std::string& file);

private:
    rtx::Mesh process_mesh(const aiMesh* mesh) const;
    rtx::Vector process_vector(const aiVector3D& vertex, bool n=false) const;
    rtx::Triangle process_face(const aiFace& face, const aiMesh* mesh,
                               const aiVector3D& normal) const;

private:
    Assimp::Importer _importer;
};

}
