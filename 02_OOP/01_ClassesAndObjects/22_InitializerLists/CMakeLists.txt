cmake_minimum_required(VERSION 3.14)

project(22_InitializerLists LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(22_InitializerLists
  main.cpp
)
target_link_libraries(22_InitializerLists Qt${QT_VERSION_MAJOR}::Core)

include(GNUInstallDirs)
install(TARGETS 22_InitializerLists
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
