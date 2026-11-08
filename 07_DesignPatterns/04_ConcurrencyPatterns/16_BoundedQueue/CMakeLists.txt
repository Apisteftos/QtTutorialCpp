cmake_minimum_required(VERSION 3.20)
project(BoundedQueue LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(bounded_queue main.cpp)
target_compile_options(bounded_queue PRIVATE -Wall -Wextra)
target_link_libraries(bounded_queue PRIVATE Threads::Threads)
