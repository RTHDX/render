project(UI-Nuklear)


create_library(
    TARGET ${PROJECT_NAME}
    SOURCES nuklear_widgets.cpp
            rtx_application.cpp
            opengl_application.cpp
            base_application.cpp
    HEADERS nuklear_widgets.hpp
            rtx_application.hpp
            opengl_application.hpp
            base_application.hpp
    LIBS UI RTX OpenGL
)
