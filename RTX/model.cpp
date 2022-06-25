#include <cassert>
#include <iostream>

#include <assimp/scene.h>
#include <assimp/vector3.h>
#include <assimp/postprocess.h>

#include "model.hpp"


namespace rtx::model {

std::vector<rtx::Mesh> Loader::read(const std::string& file) {
    const aiScene* scene = _importer.ReadFile(file,
        aiProcess_Triangulate
    );

    if (scene == nullptr) {
        std::cerr << "Unable to read mesh file: " << file << std::endl;
        return {};
    }

    std::vector<rtx::Mesh> meshes(scene->mNumMeshes);
    for (size_t i = 0; i < scene->mNumMeshes; ++i) {
        meshes[i] = process_mesh(scene->mMeshes[i]);
    }

    return meshes;
}

rtx::Mesh Loader::process_mesh(const aiMesh* mesh) const {
    std::vector<rtx::Triangle> faces(mesh->mNumFaces);
    for (size_t i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];
        assert(face.mNumIndices == 3);
        faces[i] = process_face(face, mesh, mesh->mNormals[i]);
    }

    return rtx::Mesh(faces, std::string(mesh->mName.C_Str()));
}

rtx::Triangle Loader::process_face(const aiFace& face, const aiMesh* mesh,
                                   const aiVector3D& normal) const {
    const aiVector3D* vertices = mesh->mVertices;
    return rtx::Triangle(
        process_vector(vertices[face.mIndices[0]]),
        process_vector(vertices[face.mIndices[1]]),
        process_vector(vertices[face.mIndices[2]]),
        process_vector(normal, true),
        rtx::Material()
    );
}

rtx::Vector Loader::process_vector(const aiVector3D& in, bool normalize) const {
    rtx::Vector out(in.x, in.y, in.z);
    return normalize ? glm::normalize(out) : out;
}
}
