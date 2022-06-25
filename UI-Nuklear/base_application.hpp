#pragma once

#include <string>
#include <string_view>

#include "nuklear_widgets.hpp"

struct nk_context;
struct GLFWwindow;
namespace ui::nuklear {

class BApplication {
public:
    BApplication(const std::string_view title, size_t width, size_t height);
    virtual ~BApplication();

    virtual void initialize();

protected:
    virtual bool custom_initialize() = 0;
    nk_context* ctx() { return _ctx; }
    GLFWwindow* window() { return _window; }

    void pre_render();
    void post_render();

protected:
    std::vector<uWidget> _widgets;

private:
    std::string _title;
    size_t _width, _height;
    nk_context* _ctx = nullptr;
    GLFWwindow* _window = nullptr;
};

}
