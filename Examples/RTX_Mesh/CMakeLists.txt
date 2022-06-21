cmake_minimum_required(VERSION 3.20)
project(RTX_Mesh)

create_executable(
    TARGET ${PROJECT_NAME}
    SOURCES main.cpp
    LIBS OpenGL RTX UI-Nuklear
)

copy_files(
    TARGET ${PROJECT_NAME}
    FILES "icosphere/icosphere.obj" "icosphere/icosphere.mtl"
)
