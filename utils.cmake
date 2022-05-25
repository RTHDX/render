cmake_minimum_required(VERSION 3.20)

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