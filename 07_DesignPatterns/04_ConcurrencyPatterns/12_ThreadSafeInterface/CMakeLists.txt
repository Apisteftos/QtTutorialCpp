cmake_minimum_required(VERSION 3.20)
project(ThreadSafeInterface LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(thread_safe_interface main.cpp)
target_compile_options(thread_safe_interface PRIVATE -Wall -Wextra)
target_link_libraries(thread_safe_interface PRIVATE Threads::Threads)
