cmake_minimum_required(VERSION 3.14)
project(07_Modules LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

set(GCC /usr/local/gcc-14.1.0/bin/g++-14.1.0)

# ============================================================
# Step 1: Pre-compile math.ixx
# ============================================================
add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/math.ixx.o
        ${CMAKE_CURRENT_BINARY_DIR}/gcm.cache/Math.gcm
    COMMAND ${GCC}
        -std=c++23 -fmodules-ts -x c++
        -c ${CMAKE_CURRENT_SOURCE_DIR}/math.ixx
        -o ${CMAKE_CURRENT_BINARY_DIR}/math.ixx.o
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/math.ixx
    COMMENT "Pre-compiling module: math.ixx"
)

# ============================================================
# Step 2: Pre-compile geometry.ixx (depends on math.ixx.o)
# ============================================================
add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/geometry.ixx.o
        ${CMAKE_CURRENT_BINARY_DIR}/gcm.cache/Geometry.gcm
    COMMAND ${GCC}
        -std=c++23 -fmodules-ts -x c++
        -c ${CMAKE_CURRENT_SOURCE_DIR}/geometry.ixx
        -o ${CMAKE_CURRENT_BINARY_DIR}/geometry.ixx.o
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/geometry.ixx
        ${CMAKE_CURRENT_BINARY_DIR}/math.ixx.o
    COMMENT "Pre-compiling module: geometry.ixx"
)

# ============================================================
# Step 3: Custom target for pre-compilation
# ============================================================
add_custom_target(precompile_modules
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/math.ixx.o
        ${CMAKE_CURRENT_BINARY_DIR}/geometry.ixx.o
)

# ============================================================
# Step 4: Main executable
# ============================================================
add_executable(07_Modules
    main.cpp
)

add_dependencies(07_Modules precompile_modules)

# Only apply -fmodules-ts to main.cpp — NOT to Qt autogen files
set_source_files_properties(main.cpp PROPERTIES
    COMPILE_OPTIONS "-fmodules-ts"
)

target_link_libraries(07_Modules
    Qt${QT_VERSION_MAJOR}::Core
    stdc++exp
    ${CMAKE_CURRENT_BINARY_DIR}/math.ixx.o
    ${CMAKE_CURRENT_BINARY_DIR}/geometry.ixx.o
)

include(GNUInstallDirs)
install(TARGETS 07_Modules
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
