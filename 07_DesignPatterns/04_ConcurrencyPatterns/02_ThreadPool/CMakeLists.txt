cmake_minimum_required(VERSION 3.20)
project(ThreadPool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(thread_pool main.cpp)
target_compile_options(thread_pool PRIVATE -Wall -Wextra)
target_link_libraries(thread_pool PRIVATE Threads::Threads)
