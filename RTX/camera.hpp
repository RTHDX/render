#pragma once

#include "rtx_internal.hpp"

namespace rtx {

class Camera {
public:
    Camera(const Point& position, float fov, size_t width, size_t height);

    Ray emit_ray(const size_t h_pos, const size_t w_pos) const;

    size_t width() const { return _width; }
    size_t height() const { return _height; }

    float aspect_ratio() const {
        return float(_width) / float(_height);
    }

private:
    Point ndc(size_t h_pos, size_t w_pos) const;
    Point pixel_screen(size_t h_pos, size_t w_pos) const;
    Point pixel_camera(size_t h_pos, size_t w_pos) const;

private:
    Point _position;
    float _field_of_view;
    size_t _width, _height;
};

}
