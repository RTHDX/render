#include "camera.hpp"

namespace rtx {

Camera::Camera(const Point& position, float fov, size_t width, size_t height)
    : _position(position)
    , _field_of_view(fov)
    , _width(width)
    , _height(height)
{}

Ray Camera::emit_ray(const size_t h_pos, const size_t w_pos) const {
    //float x = (2 * (i + 0.5) / (float)_width - 1) * tan(_field_of_view / 2.) * _width / (float)_width;
    //float y = (2 * (j + 0.5) / (float)_height - 1) * tan(_field_of_view / 2.);
    //Vector direction = glm::normalize(Vector(x, y, -1));
    return Ray(_position, glm::normalize(pixel_camera(h_pos, w_pos)));
}

Point Camera::ndc(size_t h_pos, size_t w_pos) const {
    return Point(
        (w_pos + 0.5) / _width,
        (h_pos + 0.5) / _height,
        -1
    );
}

Point Camera::pixel_screen(size_t h_pos, size_t w_pos) const {
    auto ndc = this->ndc(h_pos, w_pos);
    return Point(
        2 * ndc.x - 1,
        2 * ndc.y - 1,
        ndc.z
    );
}

Point Camera::pixel_camera(size_t h_pos, size_t w_pos) const {
    auto pixel_screen = this->pixel_screen(h_pos, w_pos);
    float ftan = tan(_field_of_view / 2.0f);
    return Point(
        pixel_screen.x * aspect_ratio() * ftan,
        pixel_screen.y * ftan,
        pixel_screen.z
    );
}

}
