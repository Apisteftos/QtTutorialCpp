cmake_minimum_required(VERSION 3.20)
project(Pipeline LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(pipeline main.cpp)
target_compile_options(pipeline PRIVATE -Wall -Wextra)
target_link_libraries(pipeline PRIVATE Threads::Threads)
