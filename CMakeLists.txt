cmake_minimum_required(VERSION 3.16)
project(tetris)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(tetris ${PROJECT_SOURCES})

target_include_directories(tetris PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/raylib/src
)

add_subdirectory(external/raylib)

target_link_libraries(tetris PRIVATE raylib)

if(APPLE)
    target_link_libraries(tetris PRIVATE
        "-framework CoreVideo"
        "-framework IOKit"
        "-framework Cocoa"
        "-framework GLUT"
        "-framework OpenGL"
    )
endif()

target_compile_options(tetris PRIVATE
    -Wall
    -Wextra
)
