#include <cassert>
#include <iostream>

#include <assimp/scene.h>
#include <assimp/vector3.h>
#include <assimp/postprocess.h>

#include "loader.hpp"


namespace loader {

static void dump_material(const aiMaterial* mat) {
    aiColor3D ambient, diffuse, specular;
    float alpha, reflect, transparency;
    int mod;

    mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    std::cout << "Ambient: " << ambient.r << "," << ambient.g << "," << ambient.b << std::endl;
    mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    std::cout << "Diffuse: " << diffuse.r << "," << diffuse.g << "," << diffuse.b << std::endl;
    mat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    std::cout << "Specular: " << specular.r << "," << specular.g << "," << specular.b << std::endl;
    mat->Get(AI_MATKEY_OPACITY, alpha);
    std::cout << "Alpha: " << alpha << std::endl;
    mat->Get(AI_MATKEY_REFLECTIVITY, reflect);
    std::cout << "Reflectivity: " << reflect << std::endl;
    mat->Get(AI_MATKEY_TRANSPARENCYFACTOR, transparency);
    std::cout << "Transparency: " << transparency << std::endl;
    mat->Get(AI_MATKEY_SHADING_MODEL, mod);
    std::cout << "Mod: " << mod << std::endl;

    std::cout << std::endl;
}

std::vector<Mesh> Loader::read(const std::string& path) {
    const aiScene* scene = _importer.ReadFile(path,
        aiProcess_Triangulate
    );

    if (!scene) {
        std::cerr << "Unable to read mesh file: " << path << std::endl;
        return {};
    }

    std::vector<Mesh> out(scene->mNumMeshes);
    //dump_material(scene->mMaterials[0]);
    //dump_material(scene->mMaterials[1]);
    for (size_t i = 0; i < scene->mNumMeshes; ++i) {
        out[i] = process_mesh(scene->mMeshes[i], nullptr);
    }

    return out;
}

Mesh Loader::process_mesh(const aiMesh* mesh, const aiMaterial* material) const {
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
    assert(indices.size() == 3);
    for (size_t i = 0; i < 3; ++i) {
        indices[i] = face.mIndices[i];
    }
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
