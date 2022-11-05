#include <iostream>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION
#define JSON_NOEXCEPTION
#include "tloader.hpp"


namespace loader {

bool TLoader::read(const std::filesystem::path& path) {
    tinygltf::Model model;
    std::string err;
    std::string warn;
    std::wstring temp(path.c_str());
    bool res = _loader.LoadASCIIFromFile(&model, &err, &warn,
        std::string(temp.begin(), temp.end())
    );

    if (!warn.empty()) {
        std::cerr << warn << std::endl;
        return false;
    }

    if (!err.empty()) {
        std::cerr << err << std::endl;
        return false;
    }

    if (!res) {
        std::cerr << "Could not load model " << std::endl;
        return false;
    }

    process_model(model);
    return true;
}

void TLoader::process_model(const gltf::Model& model) {
    std::cout << model.asset.generator << std::endl;

    for (const auto& node : model.nodes) {
        process_node(node, model);
    }
}

void TLoader::process_node(const gltf::Node& node, const gltf::Model& model) {
    std::cout << "Processing node: " << node.name << std::endl;
    try {
        process_mesh(model.meshes.at(node.mesh), model);
    } catch (const std::out_of_range& err) {
        std::cerr << err.what() << std::endl;
    } catch (...) {
        std::cerr << "Some error" << std::endl;
    }
}

void TLoader::process_mesh(const gltf::Mesh& mesh, const gltf::Model& model) {
    ;
}

}
