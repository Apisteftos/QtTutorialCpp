cmake_minimum_required(VERSION 3.20)
project(Noreturn LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(noreturn main.cpp)
target_compile_options(noreturn PRIVATE -Wall -Wextra)
