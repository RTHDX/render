cmake_minimum_required(VERSION 3.20)
project(Render)

option(RENDER_ENABLE_TESTING OFF)
option(RENDER_ENABLE_BENCHMARK OFF)

include(utils.cmake)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(3rdParty/assimp)
set(ASSIMP_INCLUDE_DIRECTORY)
get_target_property(ASSIMP_INCLUDE_DIRECTORY assimp INCLUDE_DIRECTORIES)
include_directories(${ASSIMP_INCLUDE_DIRECTORY})


add_subdirectory(3rdParty/glfw)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glfw")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glfw/include")
add_subdirectory(3rdParty/glad)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glad/include")
add_subdirectory(3rdParty/glm)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glm")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/Nuklear")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/Nuklear/demo/glfw_opengl4")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui/backends")
add_subdirectory(3rdParty/parselib)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty")

add_library(ImGui STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui/imgui.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui/imgui_draw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui/imgui_widgets.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui/imgui_tables.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui/backends/imgui_impl_glfw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui/backends/imgui_impl_opengl3.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui/imgui.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui/backends/imgui_impl_glfw.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui/backends/imgui_impl_opengl3.h"
)


add_subdirectory(UI)
add_subdirectory(OpenGL)
add_subdirectory(3rdParty/Nuklear/demo/glfw_opengl4)
#add_subdirectory(UI-Nuklear)
add_subdirectory(RTX)
add_subdirectory(Loader)
add_subdirectory(Examples)


if (${RENDER_ENABLE_TESTING})
    message("!!! ---Testing enabled--- !!!")
    add_subdirectory(3rdParty/googletest)
    include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/googletest/googletest/include"
    )
    include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/googletest/googlemock/include"
    )
    enable_testing()
    add_subdirectory("RTX-Test")
    add_subdirectory("Loader-Test")
endif ()

if (${RENDER_ENABLE_BENCHMARK})
    message("!!! ---Benchmark enabled--- !!!")
    add_subdirectory(3rdParty/benchmark)
    include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/benchmark/include"
    )
endif ()
