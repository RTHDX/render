cmake_minimum_required(VERSION 3.20)
project(OpenGL_SingleTriangle)

create_executable(
    TARGET OpenGL_SingleTriangle
    SOURCES main.cpp
    LIBS OpenGL UI
)
