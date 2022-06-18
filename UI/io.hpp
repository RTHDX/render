#pragma once

#include "observer.hpp"


namespace ui::io {

class IO : public ui::Publisher {
public:
    static IO& instance() {
        static IO self;
        return self;
    }

    ~IO() override = default;

private:
    IO() = default;
};


}
