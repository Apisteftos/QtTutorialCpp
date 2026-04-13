cmake_minimum_required(VERSION 3.14)

project(03_EagerSingleton LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(03_EagerSingleton
  main.cpp
)
target_link_libraries(03_EagerSingleton Qt${QT_VERSION_MAJOR}::Core)

include(GNUInstallDirs)
install(TARGETS 03_EagerSingleton
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
