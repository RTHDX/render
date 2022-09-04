cmake_minimum_required(VERSION 3.20)
project(Render)

create_library(
    TARGET ${PROJECT_NAME}
    SOURCES
        Animation.cpp
    HEADERS
        Animation.hpp
    LIBS OpenGL
)
