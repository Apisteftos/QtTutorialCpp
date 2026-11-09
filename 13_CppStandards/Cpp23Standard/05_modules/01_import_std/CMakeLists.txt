cmake_minimum_required(VERSION 3.30)

# `import std;` is still experimental in CMake (4.2). The gate value below is
# the one CMake documents for this feature; it must be set before project().
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")

project(01_import_std LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable `import std;` (needs the Ninja generator and GCC 15 or newer)
set(CMAKE_CXX_MODULE_STD ON)

add_executable(${PROJECT_NAME} main.cpp)
