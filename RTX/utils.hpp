#pragma once

#include <cmath>
#include <numbers>


namespace rtx::utils {

inline float to_radians(float degree) {
    return degree * (std::numbers::pi_v<float> / 180);
}

inline bool is_close(float num, float target) {
    return std::fabs(num - target) < std::numeric_limits<float>::epsilon();
}

}
