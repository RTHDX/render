#pragma once

#include <glm/glm.hpp>


namespace loader {

enum class IluminationModel {
    SIMPLE = 0,
    LAMBERTIAN_SHADING = 1,
    DIFFUSE_AND_SPECULAR = 2
};

struct Material {
    std::string name;
    glm::vec3 ambient_color;
    glm::vec3 diffuse_color;
    glm::vec3 specular_color;
    float specular_highlights; // 0 - 1000
    float optical_density;     // 0.001 - 10 (index of refraction)
    float dissolve;            // alpha component 0.0 - 1.0
    IluminationModel ilum;
};

}
