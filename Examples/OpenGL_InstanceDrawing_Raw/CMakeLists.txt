cmake_minimum_required(VERSION 3.20)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

create_executable(
    TARGET ${PROJECT_NAME}
    SOURCES main.cpp
    LIBS OpenGL UI ImGui Render
    SHADERS vec2tex_samp2D.frag vec3pos_vec2tex_mat4mod.vert
)

create_executable(
    TARGET "OpenGL_InstancedDrawingElements_Raw"
    SOURCES main_elements.cpp
    LIBS OpenGL UI ImGui Render
)