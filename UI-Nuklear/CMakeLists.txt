project(UI-Nuklear)


create_library(
    TARGET ${PROJECT_NAME}
    SOURCES nuklear_application.cpp
            nuklear_widgets.cpp
            rtx_application.cpp
    HEADERS nuklear_application.hpp
            nuklear_widgets.hpp
            rtx_application.hpp
    LIBS UI OpenGL
)
