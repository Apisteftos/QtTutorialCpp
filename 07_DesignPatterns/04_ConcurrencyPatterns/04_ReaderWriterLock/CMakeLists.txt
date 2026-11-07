cmake_minimum_required(VERSION 3.20)
project(ReaderWriterLock LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(reader_writer main.cpp)
target_compile_options(reader_writer PRIVATE -Wall -Wextra)
target_link_libraries(reader_writer PRIVATE Threads::Threads)
