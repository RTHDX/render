#pragma once

#include <vector>

namespace render {
using index_t = uint32_t;

class Animation {
public:
    Animation(std::vector<index_t>&& indices);

    index_t current_index() const;
    void update();

private:
    std::vector<index_t> _indices;
    size_t _current_index;
};

}
