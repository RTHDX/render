cmake_minimum_required(VERSION 3.20)
project(OpenGL)


create_library(
    TARGET ${PROJECT_NAME}
    SOURCES
        opengl_utils.cpp
        camera.cpp
        opengl_proc.cpp
        opengl_context.cpp
        opengl_vertex_input.cpp
        opengl_vertex_instance_input.cpp
        texture.cpp
        light.cpp
        image_data.cpp
        buffer_bind_guard.cpp
        opengl_render_data.cpp
        opengl_instanced_render_data.cpp
        opengl_framebuffer_data.cpp

    HEADERS
        camera.hpp
        opengl_utils.hpp
        opengl_proc.hpp
        opengl_vertex_input.hpp
        texture.hpp
        light.hpp
        texture_manager.hpp
        image_manager.hpp
        opengl_render_data.hpp
        opengl_instanced_render_data.hpp
        opengl_framebuffer_data.hpp
        opengl_context.hpp

        OpenGL.hpp

    LIBS UI
)
