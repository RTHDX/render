#pragma once

#include <vector>
#include <ranges>

namespace render {
using index_t  = uint32_t;
using frames_t = std::ranges::iota_view<index_t, index_t>;

class Animation {
public:
    Animation();
    Animation(index_t start, index_t end);

    index_t current_index() const;
    index_t total_frames() const;

    void update();
    bool is_active() const { return _is_active; }
    void active(bool value) { _is_active = value; }
    bool is_valid() const { return total_frames() > 0; }

private:
    frames_t _frames;
    size_t _offset = 0;
    bool _is_active = false;
};

}
