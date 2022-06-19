cmake_minimum_required(VERSION 3.20)
project(RTX-Test)


create_test_executable(
    TARGET "camera-test"
    SOURCES test_camera.cpp
    LIBS RTX UI
)

create_test_executable(
    TARGET "render-test"
    SOURCES test_render.cpp
    LIBS RTX UI
)
