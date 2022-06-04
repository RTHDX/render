cmake_minimum_required(VERSION 3.20)
project(RTX_Multithread)

create_executable(
    TARGET ${PROJECT_NAME}
    SOURCES main.cpp
    LIBS RTX UI OpenGL
)
