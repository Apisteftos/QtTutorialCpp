cmake_minimum_required(VERSION 3.20)
project(BarrierAndLatch LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(barrier_latch main.cpp)
target_compile_options(barrier_latch PRIVATE -Wall -Wextra)
target_link_libraries(barrier_latch PRIVATE Threads::Threads)
