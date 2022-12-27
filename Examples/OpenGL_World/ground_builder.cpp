#include <glm/gtx/transform.hpp>

#include "ground_builder.hpp"

using namespace map::reader;
namespace fs = std::filesystem;

static inline Item3D create_item(cell_t cell,
                                 uint32_t width,
                                 uint32_t height,
                                 uint32_t x_pos,
                                 uint32_t z_pos) {
    Item3D out {{
        .vertex             = fs::path("./vertex_shader.vert"),
        .fragment           = fs::path("./fragment_shader.frag"),
        .vertex_selection   = fs::path("./selection.vert"),
        .fragment_selection = fs::path("./selection.frag"),
        .color              = {0.2, 0.2, 0.2, 1.0},
        .selection_color    = {1.0, 0.0, 0.0, 1.0},
        .is_selectable      = true
    }};
    out.open("./cube.obj");
    out.modify(
        glm::scale(
            glm::translate(out.model(), {x_pos * 2, cell - 1, z_pos * 2}),
            {1.0, cell, 1.0}
        )
    );
    return out;
}

std::vector<Item3D> create_ground(const map_t& map, uint32_t w, uint32_t h) {
    uint32_t index = 0;
    std::vector<Item3D> out(w * h);
    for (uint32_t i = 0; i < w; ++i) {
        for (uint32_t j = 0; j < h; ++j) {
            out[index] = create_item(map[index], w, h, i, j);
            index++;
        }
    }
    return out;
}
