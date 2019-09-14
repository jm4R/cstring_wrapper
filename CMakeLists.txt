cmake_minimum_required(VERSION 3.5)

project(cstring_wrapper_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cstring_wrapper_test
    test/main.cpp
)

target_include_directories(cstring_wrapper_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
