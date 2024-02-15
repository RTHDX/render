cmake_minimum_required(VERSION 3.20)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})


create_executable(
    TARGET "${PROJECT_NAME}"
    SOURCES main.cpp
    HEADERS map.hpp
    LIBS OpenGL UI ImGui
    SHADERS
        pos3_uv2.vert
        pos3_uv2_mod4.vert
        vec3_vec2_mat4_f.vert

        samp2d.frag
        samp_2D_array.frag
)
