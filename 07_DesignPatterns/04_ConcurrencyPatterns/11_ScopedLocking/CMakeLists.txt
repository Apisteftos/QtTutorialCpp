cmake_minimum_required(VERSION 3.20)
project(ScopedLocking LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(scoped_locking main.cpp)
target_compile_options(scoped_locking PRIVATE -Wall -Wextra)
target_link_libraries(scoped_locking PRIVATE Threads::Threads)
