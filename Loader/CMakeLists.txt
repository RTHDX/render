cmake_minimum_required(VERSION 3.20)
project(Loader)

option(RENDER_LOADER_OPENGL_ENABLE_DEBUGOUT OFF)

#create_library(
#    TARGET "RTX-${PROJECT_NAME}"
#    SOURCES loader.cpp mesh.cpp rtx_converter.cpp material.cpp
#    HEADERS loader.hpp mesh.hpp rtx_converter.hpp material.hpp visitor.hpp
#    LIBS RTX
#)

#create_library(
#    TARGET "OpenGL-${PROJECT_NAME}"
#    SOURCES
#        loader.cpp
#        mesh.cpp
#        opengl_converter.cpp
#        material.cpp
#        tloader.cpp
#    HEADERS
#        loader.hpp
#        mesh.hpp
#        opengl_converter.hpp
#        material.hpp
#        visitor.hpp
#        tloader.hpp
#        scene.hpp
#    LIBS OpenGL parselib
#)

if (${RENDER_LOADER_OPENGL_ENABLE_DEBUGOUT})
    target_compile_definitions("OpenGL-${PROJECT_NAME}" PRIVATE "DEBUG_OUT")
endif ()
