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

create_test_executable(
    TARGET "test-object-intersections"
    SOURCES test_object_intersections.cpp
    LIBS RTX UI
)

create_test_executable(
    TARGET "test-mesh-loader"
    SOURCES test_mesh_loader.cpp
    LIBS RTX UI
)

target_compile_options("test-mesh-loader" PRIVATE
    -DCURRENT_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)
