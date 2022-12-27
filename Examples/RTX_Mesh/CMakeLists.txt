cmake_minimum_required(VERSION 3.20)
project(RTX_Mesh)

create_executable(
    TARGET ${PROJECT_NAME}
    SOURCES main.cpp
    LIBS OpenGL RTX
)

copy_files(FILES
    "stone/stone_1.obj" "stone/stone_1.mtl"
    "stone/stone_2.obj" "stone/stone_2.mtl"
    "stone/stone_3.obj" "stone/stone_3.mtl"
    "stone/stone_4.obj" "stone/stone_4.mtl"
    "stone/stone_5.obj" "stone/stone_5.mtl"
    "stone/stone_6.obj" "stone/stone_6.mtl"
)
