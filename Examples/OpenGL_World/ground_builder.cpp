#include "ground_builder.hpp"

using namespace map::reader;

static inline Item3D create_item(cell_t cell) {
    Item3D out;
    return out;
}

std::vector<Item3D> create_ground(map_t map, uint32_t w, uint32_t h) {
    uint32_t index = 0;
    std::vector<Item3D> out(w * h);
    for (uint32_t i = 0; i < w; ++i) {
        for (uint32_t j = 0; j < h; ++j) {
            ;
        }
    }
}
