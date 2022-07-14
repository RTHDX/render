#include <numbers>

#include "material.hpp"


namespace loader {

inline bool is_close(float num, float target) {
    return std::fabs(num - target) < std::numeric_limits<float>::epsilon();
}


bool operator == (const Material& lhs, const Material& rhs) {
    if (lhs.name != rhs.name) { return false; }
    if (lhs.ambient_color != rhs.ambient_color &&
        lhs.diffuse_color != rhs.diffuse_color &&
        lhs.specular_color != rhs.specular_color) { return false; }
    if (!is_close(lhs.specular_highlights, rhs.specular_highlights) &&
        !is_close(lhs.optical_density, rhs.optical_density) &&
        !is_close(lhs.dissolve, rhs.dissolve)) { return false; }
    return lhs.ilum == rhs.ilum;
}

bool operator != (const Material& lhs, const Material& rhs) {
    return !(lhs == rhs);
}

}
