#pragma once

#include <optional>
#include <functional>
#include <unordered_map>

#include "image_data.hpp"

namespace opengl {

class ImageManager final {
public:
    using image_map_t    = std::unordered_map<std::string, ImageData>;
    using iterator       = image_map_t::iterator;
    using const_iterator = image_map_t::const_iterator;

    ImageManager() = default;
    ImageManager(image_map_t&& map)
        : map_(std::move(map))
    {}

    void update(const std::string& key, const ImageData& i) {
        map_[key] = i;
    }

    std::optional<std::reference_wrapper<const ImageData>>
    get(const std::string& key) const {
        auto it = map_.find(key);
        if (it != map_.end()) {
            return std::cref(it->second);
        }
        return std::nullopt;
    }

    bool contains(const std::string& key) const {
        return map_.contains(key);
    }

    const_iterator cbegin() const {
        return map_.cbegin();
    }

    const_iterator cend() const {
        return map_.cend();
    }

    iterator begin() {
        return map_.begin();
    }

    iterator end() {
        return map_.end();
    }

private:
    image_map_t map_;
};

}
