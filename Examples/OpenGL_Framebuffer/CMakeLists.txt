cmake_minimum_required(VERSION 3.20)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})


create_executable(
    TARGET "${PROJECT_NAME}_Raw"
    SOURCES framebuffer_raw.cpp
    SHADERS
        "Shaders/vec3pos.vert"
        "Shaders/fixed_color.frag"
        "Shaders/vec3pos_vec2uv.vert"
        "Shaders/vec2uv.frag"
    LIBS OpenGL UI ImGui
)

create_executable(
    TARGET "${PROJECT_NAME}_draw_array"
    SOURCES framebuffer_draw_array.cpp
    SHADERS
        "Shaders/vec3pos_vec2uv_MVP.vert"
        "Shaders/vec3pos_MVP.vert"
    LIBS OpenGL UI ImGui
)

create_executable(
    TARGET "${PROJECT_NAME}_imgui"
    SOURCES framebuffer_imgui.cpp
    LIBS OpenGL UI ImGui
)
