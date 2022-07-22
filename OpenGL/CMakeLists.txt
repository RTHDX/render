cmake_minimum_required(VERSION 3.20)
project(OpenGL)


create_library(
    TARGET ${PROJECT_NAME}
    SOURCES
        opengl_utils.cpp
        camera.hpp
        opengl_proc.cpp
        opengl_context.cpp
    HEADERS
        opengl_utils.hpp
        camera.cpp
        opengl_proc.hpp
    LIBS UI
)
