cmake_minimum_required(VERSION 3.20)
project(Common)

create_library(
    TARGET Default-RTX-Scene
    SOURCES rtx_scene_default.cpp
    HEADERS rtx_scene_default.hpp
    LIBS RTX
)
