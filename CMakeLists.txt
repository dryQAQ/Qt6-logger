cmake_minimum_required(VERSION 3.16)

project(Qt6Logger VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

add_library(Qt6Logger STATIC
    logger.h
    logger.cpp
)

target_link_libraries(Qt6Logger PRIVATE Qt6::Core)

target_include_directories(Qt6Logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_TESTS "Build tests" OFF)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(BUILD_TESTS)
    find_package(Qt6 REQUIRED COMPONENTS Test)
    add_subdirectory(tests)
endif()
