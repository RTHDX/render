cmake_minimum_required(VERSION 3.20)
project(UI)


create_library(
	TARGET ${PROJECT_NAME}
	SOURCES ui.cpp
	HEADERS ui.hpp
)
