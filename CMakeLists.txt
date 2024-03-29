cmake_minimum_required(VERSION 3.20)
project(Render)

option(RENDER_ENABLE_TESTING OFF)
option(RENDER_ENABLE_BENCHMARK OFF)

include(utils.cmake)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")


add_subdirectory(3rdParty/glfw)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glfw")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glfw/include")
add_subdirectory(3rdParty/glad)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glad/include")
add_subdirectory(3rdParty/glm)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glm")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui/backends")
add_subdirectory(3rdParty/tinygltf)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/tinygltf")


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
add_subdirectory(Render)
option(RENDER_ENABLE_EXAMPLES OFF)
if (${RENDER_ENABLE_EXAMPLES})
    add_subdirectory(Examples)
endif()


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
    add_subdirectory("Render-Test")
endif ()


if (${RENDER_ENABLE_BENCHMARK})
    message("!!! ---Benchmark enabled--- !!!")
    add_subdirectory(3rdParty/benchmark)
    include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/benchmark/include"
    )
endif ()
