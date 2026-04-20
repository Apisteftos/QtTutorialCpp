cmake_minimum_required(VERSION 3.14)

# ============================================================
# Project declaration
# ============================================================
project(03_CMake
    VERSION     1.0.0
    DESCRIPTION "CMake tutorial project"
    LANGUAGES   CXX
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


# ============================================================
# C++ standard
# ============================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)   # disable GNU extensions — strict ISO C++

# ============================================================
# Build type — default to Release if not specified
# ============================================================
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler:   ${CMAKE_CXX_COMPILER}")

# ============================================================
# Options — user-configurable at cmake time
# cmake -DENABLE_WARNINGS=ON ..
# ============================================================
option(ENABLE_WARNINGS  "Enable extra compiler warnings" ON)
option(ENABLE_ASAN      "Enable AddressSanitizer"       OFF)

# ============================================================
# Compiler warnings
# ============================================================
if(ENABLE_WARNINGS)
    if(MSVC)
        add_compile_options(/W4)
    else()
        add_compile_options(-Wall -Wextra -Wpedantic)
    endif()
endif()

# ============================================================
# AddressSanitizer
# ============================================================
if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
    message(STATUS "AddressSanitizer: ON")
endif()


# ============================================================
# Qt (optional — comment out if not using Qt)
# ============================================================
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)


# ============================================================
# Executable
# ============================================================
add_executable(03_CMake
  main.cpp
)


# ============================================================
# Link libraries
# ============================================================
target_link_libraries(03_CMake Qt${QT_VERSION_MAJOR}::Core)


# ============================================================
# Compile definitions
# ============================================================
target_compile_definitions(03_CMake PRIVATE
    PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
)

# ============================================================
# Install rules
# ============================================================
include(GNUInstallDirs)
install(TARGETS 03_CMake
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)



# ============================================================
# Print summary
# ============================================================
message(STATUS "")
message(STATUS "=== Build configuration ===")
message(STATUS "  Project:  ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "  Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Type:     ${CMAKE_BUILD_TYPE}")
message(STATUS "  Warnings: ${ENABLE_WARNINGS}")
message(STATUS "  ASan:     ${ENABLE_ASAN}")
message(STATUS "===========================")

