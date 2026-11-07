cmake_minimum_required(VERSION 3.20)
project(ActiveObject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(active_object main.cpp)
target_compile_options(active_object PRIVATE -Wall -Wextra)
target_link_libraries(active_object PRIVATE Threads::Threads)
