cmake_minimum_required(VERSION 3.20)
project(02_unreachable_and_stacktrace LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} main.cpp)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

# GCC 14 and 15 keep std::stacktrace in a separate library
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 14)
    target_link_libraries(${PROJECT_NAME} PRIVATE stdc++exp)
endif()
