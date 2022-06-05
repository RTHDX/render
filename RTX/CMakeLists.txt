cmake_minimum_required(VERSION 3.20)
project(RTX)


create_library(
    TARGET ${PROJECT_NAME}
    SOURCES rtx_internal.cpp render.cpp camera.cpp
    HEADERS rtx_internal.hpp render.hpp camera.hpp utils.hpp
)
