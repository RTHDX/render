cmake_minimum_required(VERSION 3.20)
project(OpenGL)


create_library(
    TARGET ${PROJECT_NAME}
    SOURCES
        opengl_utils.cpp
        camera.hpp
        opengl_proc.cpp
        opengl_context.cpp
        opengl_vertex_input.cpp
        ortho_camera.cpp
        texture.cpp
        opengl_functions.cpp
        light.cpp
    HEADERS
        opengl_utils.hpp
        camera.cpp
        opengl_proc.hpp
        opengl_vertex_input.hpp
        texture.hpp
        opengl_functions.hpp
        light.hpp
    LIBS UI
)
