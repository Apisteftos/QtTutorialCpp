cmake_minimum_required(VERSION 3.20)
project(ThreadSpecificStorage LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(thread_specific main.cpp)
target_compile_options(thread_specific PRIVATE -Wall -Wextra)
target_link_libraries(thread_specific PRIVATE Threads::Threads)
