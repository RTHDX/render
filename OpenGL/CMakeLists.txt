cmake_minimum_required(VERSION 3.20)
project(OpenGL)


create_library(
    TARGET ${PROJECT_NAME}
    SOURCES
        opengl_utils.cpp
        camera.hpp
        opengl_proc.cpp
        opengl_context.cpp
        opengl_vertex_input.cpp
        opengl_vertex_instance_input.cpp
        ortho_camera.cpp
        texture.cpp
        opengl_functions.cpp
        light.cpp
        image_data.cpp
        buffer_bind_guard.cpp
        opengl_render_data.cpp
        opengl_instanced_render_data.cpp
        _camera.cpp

    HEADERS
        opengl_utils.hpp
        camera.cpp
        opengl_proc.hpp
        opengl_vertex_input.hpp
        texture.hpp
        opengl_functions.hpp
        light.hpp
        texture_manager.hpp
        image_manager.hpp
        opengl_render_data.hpp
        opengl_instanced_render_data.hpp
        _camera.hpp

    LIBS UI
)
