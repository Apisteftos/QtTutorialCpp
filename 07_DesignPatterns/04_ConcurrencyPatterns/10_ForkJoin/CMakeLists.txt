cmake_minimum_required(VERSION 3.20)
project(ForkJoin LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(fork_join main.cpp)
target_compile_options(fork_join PRIVATE -Wall -Wextra)
target_link_libraries(fork_join PRIVATE Threads::Threads)
