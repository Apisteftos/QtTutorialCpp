cmake_minimum_required(VERSION 3.20)
project(GuardedSuspensionAndBalking LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(guarded_balking main.cpp)
target_compile_options(guarded_balking PRIVATE -Wall -Wextra)
target_link_libraries(guarded_balking PRIVATE Threads::Threads)
