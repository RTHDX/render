#pragma once

#include "rtx_internal.hpp"
#include "camera.hpp"
#include "render.hpp"
#include "model.hpp"

#define BENCHMARK(FUNCTION_CALL)                        \
    {                                                   \
        auto before = std::chrono::system_clock::now(); \
        FUNCTION_CALL;                                  \
        auto after = std::chrono::system_clock::now();  \
        std::cout << (after - before) << std::endl;     \
    }
