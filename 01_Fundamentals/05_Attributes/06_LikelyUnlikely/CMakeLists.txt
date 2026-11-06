cmake_minimum_required(VERSION 3.20)
project(LikelyUnlikely LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(likelyunlikely main.cpp)
target_compile_options(likelyunlikely PRIVATE -Wall -Wextra)
