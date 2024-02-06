cmake_minimum_required(VERSION 3.20)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

#create_executable(
#    TARGET ${PROJECT_NAME}
#    SOURCES main.cpp
#    LIBS OpenGL UI
#)

copy_files(FILES "vertex_shader.vert" "fragment_shader.frag"
                 "hr-level1_running.png" "fire.jpg" "test_1.png")

create_executable(
    TARGET "${PROJECT_NAME}_2"
    SOURCES main_2.cpp
    LIBS OpenGL UI
)
