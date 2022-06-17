project(UI-Nuklear)


create_library(
    TARGET ${PROJECT_NAME}
    SOURCES nuklear_application.cpp
            nuklear_widgets.cpp
    HEADERS nuklear_application.hpp
            nuklear_widgets.hpp
    LIBS UI OpenGL
)
