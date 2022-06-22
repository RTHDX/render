#pragma once

#include <string>
#include <string_view>

namespace ui::nuklear {

class RTX_App {
public:
    RTX_App() = delete;
    RTX_App(const std::string_view title, size_t width, size_t height);

    bool initialize();
    void run();

private:
    std::string _title;
    size_t _width, _height;
};

}
