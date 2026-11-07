cmake_minimum_required(VERSION 3.20)
project(ProducerConsumer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(producer_consumer main.cpp)
target_compile_options(producer_consumer PRIVATE -Wall -Wextra)
target_link_libraries(producer_consumer PRIVATE Threads::Threads)
