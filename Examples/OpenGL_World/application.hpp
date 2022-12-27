#pragma once

#include "item.hpp"


class Application {
public:
    Application(Scene&& scene);

private:
    Scene _scene;
};
