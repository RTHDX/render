cmake_minimum_required(VERSION 3.20)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})


create_executable(
    TARGET ${PROJECT_NAME}
    SOURCES main.cpp item.cpp io.cpp map_reader.cpp
    HEADERS item.hpp io.hpp map_reader.hpp
    LIBS OpenGL UI OpenGL-Loader
    MESHES cube.obj cube.mtl
    SHADERS fragment_shader.frag vertex_shader.vert selection.vert
            selection.frag
)
