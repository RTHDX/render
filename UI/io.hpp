#pragma once

#include "observer.hpp"


struct GLFWwindow;
namespace ui::io {

class IO : public ui::Publisher {
public:
    static IO& instance() {
        static IO self;
        return self;
    }

    ~IO() override = default;

    void bind(GLFWwindow* window);
    bool is_bound() const { return _is_bound; }

private:
    IO() = default;

private:
    bool _is_bound = false;
};


}
