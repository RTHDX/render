#pragma once

#include <vector>

#include <assimp/Importer.hpp>

#include "item.hpp"

struct aiMesh;
struct aiFace;
namespace opengl::model {

class Loader {
public:
    Loader();

    std::vector<Item> raad(const std::string& path);

private:
    Item process_mesh(const aiMesh)
};

}
