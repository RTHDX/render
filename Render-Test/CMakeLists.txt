cmake_minimum_required(VERSION 3.20)
project(Render-Test)

create_test_executable(
	TARGET animation_test
	SOURCES test_animation.cpp
	LIBS Render
)
