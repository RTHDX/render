#include <cassert>
#include <iostream>

#include <assimp/scene.h>
#include <assimp/vector3.h>
#include <assimp/postprocess.h>

#include "loader.hpp"


namespace loader {

std::vector<Mesh> Loader::read(const std::string& path) {
    const aiScene* scene = _importer.ReadFile(path,
        aiProcess_Triangulate
    );

    if (!scene) {
        std::cerr << "Unable to read mesh file: " << path << std::endl;
        return {};
    }

    std::vector<Mesh> out(scene->mNumMeshes);
    for (size_t i = 0; i < scene->mNumMeshes; ++i) {
        out[i] = process_mesh(scene->mMeshes[i]);
    }

    return out;
}

Mesh Loader::process_mesh(const aiMesh* mesh) const {
    Mesh out;
    std::vector<Face> faces(mesh->mNumFaces);
    for (size_t i = 0; i < faces.size(); ++i) {
        faces[i] = process_face(mesh->mFaces[i], &out, i);
    }
    std::vector<glm::vec3> vertices(mesh->mNumVertices);
    for (size_t i = 0; i < vertices.size(); ++i) {
        vertices[i] = process_vector(mesh->mVertices[i]);
    }

    std::vector<glm::vec3> normals(mesh->mNumFaces);
    for (size_t i = 0; i < normals.size(); ++i) {
        normals[i] = process_vector(mesh->mNormals[i], true);
    }

    out.vertices = vertices;
    out.normals = normals;
    out.faces = faces;
    return out;
}

Face Loader::process_face(const aiFace& face, Mesh* mesh,
                          size_t normal_index) const {
    std::vector<size_t> indices(face.mNumIndices);
    return {
        std::move(indices),
        normal_index,
        mesh
    };
}

glm::vec3 Loader::process_vector(const aiVector3D& in, bool n) const {
    glm::vec3 out(in.x, in.y, in.z);
    return n ? glm::normalize(out) : out;
}

}
