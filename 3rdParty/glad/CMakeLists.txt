cmake_minimum_required(VERSION 3.15)
project(glad)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
add_library(${PROJECT_NAME} src/glad.c)
