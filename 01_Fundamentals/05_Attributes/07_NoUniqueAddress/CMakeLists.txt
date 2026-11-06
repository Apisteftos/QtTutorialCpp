cmake_minimum_required(VERSION 3.20)
project(NoUniqueAddress LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(nouniqueaddress main.cpp)
target_compile_options(nouniqueaddress PRIVATE -Wall -Wextra)
