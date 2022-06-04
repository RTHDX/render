#pragma once

#include "rtx.hpp"

namespace rtx {
using Framebuffer = std::vector<Color>;

class Render {
    static constexpr float DELTA = 0.0001;
public:
    Render(const Scene& scene, const Color& background, const Camera& camera);

    void render();
    size_t width() const { return _camera.width(); }
    size_t height() const { return _camera.height(); }
    const Framebuffer& buffer() const { return _framebuffer; }

private:
    Color cast_ray(const Ray& ray, size_t depth = 0) const;
    size_t eval_index(const size_t h_pos, const size_t w_pos) const;
    Ray reflection_ray(const Ray& primary, const Hit& last_hit) const;
    Ray refraction_ray(const Ray& primary, const Hit& last_hit) const;
    bool is_shaded(const Light& light, const Hit& last_hit) const;

private:
    Scene _scene;
    Framebuffer _framebuffer;
    Color _background;
    Camera _camera;
};

}
