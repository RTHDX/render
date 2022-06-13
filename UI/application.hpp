#pragma once

#include <string_view>

#include "observer.hpp"

struct GLFWwindow;
namespace ui {

class Application : public Publisher {
public:
    static Application& instance();

    void title(const std::string_view title);
    GLFWwindow* window() const;

private:
    Application();

private:
    GLFWwindow* _window;
};

}
