#pragma once

#include <string>

#include <glm/glm.hpp>


namespace loader {

enum class IluminationModel {
    SIMPLE = 0,
    LAMBERTIAN_SHADING = 1,
    DIFFUSE_AND_SPECULAR = 2
};

struct Material {
    std::string name;
    glm::vec3 ambient_color;   // Ka
    glm::vec3 diffuse_color;   // Kd
    glm::vec3 specular_color;  // Ks
    float specular_highlights; // 0 - 1000 (Ns)
    float optical_density;     // 0.001 - 10 (Ni)
    float dissolve;            // alpha component 0.0 - 1.0 (d)
    IluminationModel ilum;
};

bool operator == (const Material& lha, const Material& rhs);
bool operator != (const Material& lhs, const Material& rhs);

}
