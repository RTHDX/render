cmake_minimum_required(VERSION 3.20)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

create_executable(
	TARGET "OpenGL_InstancedDrawing_Array"
	SOURCES main_array_draw.cpp
	SHADERS vec2tex_samp2D.frag vec3pos_vec2tex_mat4mod.vert
	LIBS OpenGL UI ImGui Render
)

create_executable(
	TARGET "OpenGL_InstancedDrawing_Elements"
	SOURCES main_elements_draw.cpp
	LIBS OpenGL UI ImGui Render
)
