cmake_minimum_required(VERSION 3.20)
project(RTX_Simple)

create_executable(
    TARGET ${PROJECT_NAME}
    SOURCES main.cpp
    LIBS OpenGL UI
)
