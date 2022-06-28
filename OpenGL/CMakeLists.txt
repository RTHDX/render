cmake_minimum_required(VERSION 3.20)
project(OpenGL)


create_library(
    TARGET ${PROJECT_NAME}
    SOURCES opengl.cpp opengl_utils.cpp item.cpp camera.hpp model.cpp
    HEADERS opengl.hpp opengl_utils.hpp item.hpp camera.cpp model.hpp
    LIBS UI
)
