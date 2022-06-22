cmake_minimum_required(VERSION 3.20)
project(RTX-Test)


copy_files(
    FILES mesh-examples/icosphere/icosphere.obj
          mesh-examples/icosphere/icosphere.mtl
          mesh-examples/cube/cube.obj
          mesh-examples/cube/cube.mtl
)

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
