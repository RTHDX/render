cmake_minimum_required(VERSION 3.20)
project(Render)

include(utils.cmake)

add_subdirectory(3rdParty/glfw)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glfw")
add_subdirectory(3rdParty/glad)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glad/include")
add_subdirectory(3rdParty/glm)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glm")

add_subdirectory(OpenGL)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/OpenGL")

add_subdirectory(UI)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/UI")

create_executable(
    TARGET render
    SOURCES main.cpp
    LIBS OpenGL UI
)
