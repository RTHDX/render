#pragma once

#include "item.hpp"
#include "map_reader.hpp"

std::vector<Item3D> create_ground(const map::reader::map_t&,
                                  uint32_t w,
                                  uint32_t h);
