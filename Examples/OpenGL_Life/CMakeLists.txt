cmake_minimum_required(VERSION 3.20)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME} LANGUAGES CXX CUDA)


create_executable(
	TARGET ${PROJECT_NAME}
	SOURCES main.cpp
	LIBS OpenGL UI
)

copy_files(FILES "vertex_shader.vert" "fragment_shader.frag")
