cmake_minimum_required(VERSION 3.20)

function (copy_shaders)
    cmake_parse_arguments(THIS "" "TARGET" "SHADERS" ${ARGV})
    foreach (SHADER ${THIS_SHADERS})
        file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER}"
             DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
    endforeach ()
endfunction()

function (include_deps)
    cmake_parse_arguments(THIS "" "TARGET" "DEPS" ${ARGV})
    message("Searching source folders of: ${THIS_DEPS} for: ${THIS_TARGET}")
    foreach (DEP ${THIS_DEPS})
        set(DEPS_INCL_DIR)
        get_target_property(DEPS_INCL_DIR ${DEP} SOURCE_DIR)
        target_include_directories(${THIS_TARGET} PUBLIC ${DEP_INCL_DIR})
        message("---------------> dirs: ${DEPS_INCL_DIR}")
    endforeach ()
    set(OUT_INC_DIRS)
    get_target_property(OUT_INC_DIRS ${THIS_TARGET} INCLUDE_DIRECTORIES)
    message("Include dirs: ${OUT_INC_DIRS}")
endfunction ()

function (create_executable)
    set(CMAKE_CUDA_ARCHITECTURES CMAKE_CUDA_ARCHITECTURES_DEFAULT)

    cmake_parse_arguments(THIS "" "TARGET" "HEADERS;SOURCES;LIBS" ${ARGV})
    message("Preparing executable: ${THIS_TARGET}")
    add_executable(${THIS_TARGET} ${THIS_HEADERS} ${THIS_SOURCES})
    target_include_directories(${THIS_TARGET} PRIVATE
                               ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
    target_link_libraries(${THIS_TARGET} PRIVATE
                          ${OPENGL_LIBRARIES}
                          glfw
                          glad
                          ${THIS_LIBS}
    )
    set_property(TARGET ${THIS_TARGET} PROPERTY CXX_STANDARD 20)
    set_property(TARGET ${THIS_TARGET} PROPERTY CUDA_SEPARABLE_COMPILATION ON)
endfunction (create_executable)


function (create_library)
    set(CMAKE_CUDA_ARCHITECTURES CMAKE_CUDA_ARCHITECTURES_DEFAULT)

    cmake_parse_arguments(THIS "" "TARGET" "HEADERS;SOURCES;LIBS" ${ARGV})
    message("Preparing library: ${THIS_TARGET}")
    add_library(${THIS_TARGET} STATIC ${THIS_HEADERS} ${THIS_SOURCES})
    target_include_directories(${THIS_TARGET} PRIVATE
                               ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
    target_link_libraries(${THIS_TARGET} PRIVATE
                          ${OPENGL_LIBRARIES}
                          glfw
                          glad
                          ${THIS_LIBS}
    )
    set_property(TARGET ${THIS_TARGET} PROPERTY CXX_STANDARD 20)
    set_property(TARGET ${THIS_TARGET} PROPERTY CUDA_SEPARABLE_COMPILATION ON)
endfunction (create_library)


function (create_test_executable)
    cmake_parse_arguments(THIS "" "TARGET" "HEADERS;SOURCES;LIBS" ${ARGV})
    create_executable(
        TARGET  ${THIS_TARGET}
        SOURCES ${THIS_SOURCES}
        HEADERS ${THIS_HEADERS}
        LIBS ${THIS_LIBS} GTest::gtest GTest::gtest_main GTest::gmock
                          GTest::gmock_main)
endfunction(create_test_executable)
