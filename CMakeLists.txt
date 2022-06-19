cmake_minimum_required(VERSION 3.20)
project(Render)

option(RENDER_ENABLE_TESTING OFF)

include(utils.cmake)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(3rdParty/glfw)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glfw")
add_subdirectory(3rdParty/glad)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glad/include")
add_subdirectory(3rdParty/glm)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glm")
add_subdirectory(3rdParty/assimp)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/assimp/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/Nuklear")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/Nuklear/demo/glfw_opengl4")

add_subdirectory(UI)
add_subdirectory(OpenGL)
add_subdirectory(3rdParty/Nuklear/demo/glfw_opengl4)
add_subdirectory(UI-Nuklear)
add_subdirectory(RTX)
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
endif ()
