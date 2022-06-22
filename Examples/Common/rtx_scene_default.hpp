#pragma once

#include <vector>

#include <RTX/rtx.hpp>
#include <OpenGL/opengl.hpp>
#include <RTX/utils.hpp>
#include <RTX/rtx_internal.hpp>

namespace rtx {

Material mirror();
Material ivory();
Material red_rubber();
Material glass();

Sphere make_ground(const Material& material, float radius);

static constexpr size_t WIDTH = 1280;
static constexpr size_t HEIGHT = 860;

std::vector<Sphere> make_spheres();
std::vector<Light> make_lights();
Camera make_camera();
Scene<Sphere> make_scene();

}
