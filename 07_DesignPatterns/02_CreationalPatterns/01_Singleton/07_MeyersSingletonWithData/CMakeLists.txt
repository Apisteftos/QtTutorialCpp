cmake_minimum_required(VERSION 3.20)
project(MeyersSingletonWithData LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(meyers_data main.cpp)
target_compile_options(meyers_data PRIVATE -Wall -Wextra)
target_link_libraries(meyers_data PRIVATE Threads::Threads)
