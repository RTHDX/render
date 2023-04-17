cmake_minimum_required(VERSION 3.20)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})


create_executable(
    TARGET ${PROJECT_NAME}
    SOURCES infinite_grid_main.cpp
    HEADERS infinite_grid_decls.hpp
    SHADERS grid_pos3.vert
            gpt_pos3.vert
            grid.frag
            gpt_grid.frag
    LIBS OpenGL UI
)
