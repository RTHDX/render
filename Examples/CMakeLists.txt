cmake_minimum_required(VERSION 3.20)

add_subdirectory(Common)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(OpenGL_SingleTriangle)
add_subdirectory(OpenGL_MultipleTriangle)
add_subdirectory(OpenGL_Cube)
add_subdirectory(RTX_Simple)
add_subdirectory(RTX_Simple_2)
add_subdirectory(RTX_Multithread)
add_subdirectory(RTX_Camera_Movable)
