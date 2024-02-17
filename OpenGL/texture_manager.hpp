#pragma once

#include <optional>
#include <functional>
#include <unordered_map>

#include "texture.hpp"

namespace opengl {

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;


class TextureManager final {
public:
    using texture_map_t  = std::unordered_map<std::string, any_texture_t>;
    using iterator       = texture_map_t::iterator;
    using const_iterator = texture_map_t::const_iterator;

    template <typename T> using maybe_texture_t = std::optional<
        std::reference_wrapper<const T>
    >;

    TextureManager() = default;
    TextureManager(texture_map_t&& map)
        : map_(std::move(map))
    {}

    ~TextureManager() {
        for (auto& [key, any_texture] : map_) {
            std::visit(overloaded {
                [](texture_data_t& tex)          { tex.free(); },
                [](texture_data_array_2d_t& tex) { tex.free(); }
            }, any_texture);
        }
    }

    void update(const std::string& key, const any_texture_t& val) {
        map_[key] = val;
    }

    void update(const std::string& key, any_texture_t&& val) {
        map_[key] = std::move(val);
    }

    template<typename T> maybe_texture_t<T> get(const std::string& key) const {
        if (!map_.contains(key)) {
            return std::nullopt;
        }
        const any_texture_t& tex = map_.at(key);
        if (const T* val = std::get_if<T>(&tex)) {
            return std::cref(*val);
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
    texture_map_t map_;
};

}
