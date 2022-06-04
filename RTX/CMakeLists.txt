cmake_minimum_required(VERSION 3.20)
project(RTX)


create_library(
    TARGET ${PROJECT_NAME}
    SOURCES rtx.cpp render.cpp
    HEADERS rtx.hpp utils.hpp render.hpp
)
