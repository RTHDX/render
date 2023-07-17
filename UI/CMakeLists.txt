cmake_minimum_required(VERSION 3.20)
project(UI)

create_library(
    TARGET ${PROJECT_NAME}
    SOURCES
        ui.cpp
        application.cpp
        observer.cpp
        io.cpp
        widgets.cpp
        imgui_widget_render.cpp
    HEADERS
        ui.hpp
        application.hpp
        observer.hpp
        property.hpp
        io.hpp
        widgets.hpp
        imgui_widget_render.hpp
    LIBS ImGui
)
