cmake_minimum_required(VERSION 3.16)

project(logger_basic_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

add_executable(logger_basic_example
    main.cpp
)

target_link_libraries(logger_basic_example PRIVATE Qt6::Core Qt6Logger)
target_include_directories(logger_basic_example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../..)
