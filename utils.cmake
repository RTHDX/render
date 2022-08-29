cmake_minimum_required(VERSION 3.20)

set(RENDER_BOOST_INCLUDE_DIR "" CACHE PATH
    "Variable with boost library include path")
set(RENDER_BOOST_LINK_DIR "" CACHE PATH
    "Variable with boost library link path")


function (copy_files)
    cmake_parse_arguments(THIS "" "" "FILES" ${ARGV})
    foreach (FILE ${THIS_FILES})
        message("Copying ${FILE}")
        file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/${FILE}"
             DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
    endforeach ()
endfunction()

function (include_deps)
    cmake_parse_arguments(THIS "" "TARGET" "DEPS" ${ARGV})
    foreach (DEP ${THIS_DEPS})
        set(DEPS_INCL_DIR)
        get_target_property(DEPS_INCL_DIR ${DEP} INCLUDE_DIRECTORIES)
        target_include_directories(${THIS_TARGET} PRIVATE ${DEP_INCL_DIR})
    endforeach ()
endfunction ()

function (create_executable)
    #set(CMAKE_CUDA_ARCHITECTURES CMAKE_CUDA_ARCHITECTURES_DEFAULT)

    cmake_parse_arguments(THIS "" "TARGET" "HEADERS;SOURCES;LIBS" ${ARGV})
    message("Preparing executable: ${THIS_TARGET}")
    add_executable(${THIS_TARGET} ${THIS_HEADERS} ${THIS_SOURCES})
    target_include_directories(${THIS_TARGET} PRIVATE ${RENDER_BOOST_INCLUDE_DIR})
    target_link_libraries(${THIS_TARGET} PRIVATE
                          ${OPENGL_LIBRARIES}
                          glfw
                          glad
                          assimp
                          ${THIS_LIBS}
    )
    target_link_directories(${THIS_TARGET} PRIVATE ${RENDER_BOOST_LINK_DIR})
    set_property(TARGET ${THIS_TARGET} PROPERTY CXX_STANDARD 20)
    target_compile_definitions(${THIS_TARGET} PRIVATE
                               "DEBUG=$<IF:$<CONFIG:Debug>,1,0>")
endfunction (create_executable)


function (create_library)
    #set(CMAKE_CUDA_ARCHITECTURES CMAKE_CUDA_ARCHITECTURES_DEFAULT)

    cmake_parse_arguments(THIS "" "TARGET" "HEADERS;SOURCES;LIBS" ${ARGV})
    message("Preparing library: ${THIS_TARGET}")
    add_library(${THIS_TARGET} STATIC ${THIS_HEADERS} ${THIS_SOURCES})
    target_include_directories(${THIS_TARGET} PRIVATE
                               ${RENDER_BOOST_INCLUDE_DIR})
    target_link_libraries(${THIS_TARGET} PRIVATE
                          ${OPENGL_LIBRARIES}
                          glfw
                          glad
                          ${THIS_LIBS}
    )
    target_link_directories(${THIS_TARGET} PRIVATE ${RENDER_BOOST_LINK_DIR})
    set_property(TARGET ${THIS_TARGET} PROPERTY CXX_STANDARD 20)
    target_compile_definitions(${THIS_TARGET} PRIVATE
                               "DEBUG=$<IF:$<CONFIG:Debug>,1,0>")
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
