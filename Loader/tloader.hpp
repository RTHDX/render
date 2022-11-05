#pragma once

#include <filesystem>

#include <tiny_gltf.h>


namespace loader {
namespace gltf = tinygltf;

class TLoader {
public:
    TLoader() = default;

    bool read(const std::filesystem::path& path);

private:
    void process_model(const gltf::Model& model);
    void process_node(const gltf::Node& node, const gltf::Model& model);
    void process_mesh(const gltf::Mesh& mesh, const gltf::Model& model);
    void process_primitive(const gltf::Primitive& prim, const gltf::Model& mod);

private:
    tinygltf::TinyGLTF _loader;
};

}
