cmake_minimum_required(VERSION 3.20)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

create_executable(
    TARGET ${PROJECT_NAME}
    SOURCES main.cpp ui-imgui.hpp
    LIBS OpenGL UI ImGui OpenGL-Loader
)

copy_files(FILES vertex_shader.vert fragment_shader.frag)
copy_files(FILES "meshes/cube.obj" "meshes/cube.mtl")
