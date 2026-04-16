cmake_minimum_required(VERSION 3.14)
project(04_BenchmarkingAndProfiling LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

find_package(benchmark REQUIRED)          # ← ADD THIS

add_executable(04_BenchmarkingAndProfiling
    main.cpp
)

target_link_libraries(04_BenchmarkingAndProfiling
    Qt${QT_VERSION_MAJOR}::Core
    benchmark::benchmark                  # ← ADD THIS
    pthread                               # ← ADD THIS
)

include(GNUInstallDirs)
install(TARGETS 04_BenchmarkingAndProfiling
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
