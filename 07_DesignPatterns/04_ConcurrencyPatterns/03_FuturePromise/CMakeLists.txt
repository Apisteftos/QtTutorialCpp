cmake_minimum_required(VERSION 3.20)
project(FuturePromise LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(future_promise main.cpp)
target_compile_options(future_promise PRIVATE -Wall -Wextra)
target_link_libraries(future_promise PRIVATE Threads::Threads)
