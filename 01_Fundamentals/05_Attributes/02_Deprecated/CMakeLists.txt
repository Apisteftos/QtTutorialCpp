cmake_minimum_required(VERSION 3.20)
project(Deprecated LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(deprecated main.cpp)
target_compile_options(deprecated PRIVATE -Wall -Wextra)
