cmake_minimum_required(VERSION 3.20)
project(Loader_Test)


copy_files(FILES
    "mesh-examples/stone_1.obj" "mesh-examples/stone_1.mtl"
    "mesh-examples/stone_2.obj" "mesh-examples/stone_2.mtl"
    "mesh-examples/stone_3.obj" "mesh-examples/stone_3.mtl"
    "mesh-examples/stone_4.obj" "mesh-examples/stone_4.mtl"
    "mesh-examples/stone_5.obj" "mesh-examples/stone_5.mtl"
    "mesh-examples/stone_6.obj" "mesh-examples/stone_6.mtl"
)

create_test_executable(
    TARGET "OpenGL-Loader-Test"
    SOURCES opengl_test.cpp
    LIBS "OpenGL-Loader"
)
