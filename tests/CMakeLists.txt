cmake_minimum_required(VERSION 3.16)

project(logger_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Test)
qt_standard_project_setup()

add_executable(logger_tests
    test_logger.cpp
)

target_link_libraries(logger_tests PRIVATE Qt6::Core Qt6::Test Qt6Logger)
target_include_directories(logger_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

add_test(NAME logger_tests COMMAND logger_tests)
