#pragma once

#include <thread>

#include "rtx_internal.hpp"
#include "camera.hpp"

namespace rtx {
using Framebuffer = std::vector<Color>;

class BaseRender {
public:
    BaseRender(const Scene& scene, const Color& background,
               const Camera& camera);
    virtual ~BaseRender() = default;

    virtual void render() = 0;

    size_t width() const { return _camera.width(); }
    size_t height() const { return _camera.height(); }
    const Framebuffer& buffer() const { return _framebuffer; }

protected:
    Color cast_ray(const Ray& ray, size_t depth = 0) const;
    size_t eval_index(const size_t h_pos, const size_t w_pos) const;
    Ray reflection_ray(const Ray& primary, const Hit& last_hit) const;
    Ray refraction_ray(const Ray& primary, const Hit& last_hit) const;
    bool is_shaded(const Light& light, const Hit& last_hit) const;

protected:
    static constexpr float DELTA = 0.0001;

    Scene _scene;
    Framebuffer _framebuffer;
    Color _background;
    Camera _camera;
    CameraListener _cam_handler;
};


class Render final : public BaseRender {
public:
    Render(const Scene& scene, const Color& background, const Camera& camera);
    ~Render() override = default;

    void render() override;
};


class MultiThreadRender final : public BaseRender {
public:
    MultiThreadRender(const Scene& scene, const Color& background,
                      const Camera& camera);

    void render() override;

private:
    void partial_rander(size_t w_start, size_t w_end);

private:
    std::vector<std::thread> _workers;
    size_t _step;
};

}
