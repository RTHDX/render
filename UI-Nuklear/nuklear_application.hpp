#pragma once

#include <string_view>
#include <functional>

#include "nuklear_widgets.hpp"

struct nk_context;
struct GLFWwindow;
namespace ui::nuklear {
using RenderCallback = std::function<void(void)>;

class Application {
public:
    Application(const std::string_view title, size_t w=800, size_t h=600);
    Application(const Application&) = delete;
    Application& operator = (const Application&) = delete;
    ~Application();

    void run(RenderCallback& render);

    GLFWwindow* window() { return _window; }
    nk_context* ctx() { return _ctx; }
    void append(uWidget&& widget);

private:
    size_t _width, _height;
    std::string _title;
    GLFWwindow* _window = nullptr;
    nk_context* _ctx;

    std::vector<uWidget> _widgets;
};

}
