cmake_minimum_required(VERSION 3.20)
project(STB)

create_library(
	TARGET ${PROJECT_NAME}
	SOURCES impl.cpp
)
