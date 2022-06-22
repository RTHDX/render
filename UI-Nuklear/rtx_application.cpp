#include "rtx_application.hpp"

namespace ui::nuklear {

RTX_App::RTX_App(const std::string_view title, size_t width, size_t height)
    : _title(title)
    , _width(width)
    , _height(height)
{}

bool RTX_App::initialize() {
    return false;
}

void RTX_App::run() {
    ;
}

}
