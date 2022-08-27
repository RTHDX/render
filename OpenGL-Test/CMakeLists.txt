cmake_minimum_required(VERSION 3.20)
project(OpenGL_Test)

create_test_executable(
	TARGET texture_test
	SOURCES texture_test.cpp
	HEADERS
	LIBS OpenGL UI
)

copy_files(FILES "hr-level1_running.png")
