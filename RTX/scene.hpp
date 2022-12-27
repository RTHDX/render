#pragma once

#include <vector>
#include <type_traits>

#include "rtx_internal.hpp"

namespace rtx {

template <typename T> concept traceable_c = std::is_base_of<Object, T>::value;


template <traceable_c T> struct Scene final : public Object {
    std::vector<T> objects;
    std::vector<Light> lights;
    size_t active_ray_count = 0;

public:
    Scene() = default;
    Scene(std::vector<T>&& objects, std::vector<Light>&& lights)
        : objects(std::move(objects))
        , lights(std::move(lights))
    {}

    Hit ray_intersect(const Ray& ray) const override {
        static constexpr float LIMIT = 1000.0;
        float total_dist = std::numeric_limits<float>::max();

        Hit out_hit;
        for (const auto& object : objects) {
            Hit hit = object.ray_intersect(ray);
            if (hit.is_valid() && hit.t < total_dist) {
                total_dist = hit.t;
                out_hit = hit;
            }
        }

        return total_dist < LIMIT ? out_hit : Hit();
    }
};

}
