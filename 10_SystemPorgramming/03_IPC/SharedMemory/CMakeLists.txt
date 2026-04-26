cmake_minimum_required(VERSION 3.14)

project(SharedMemory LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(client_a client_a.cpp shared_data.h)
target_link_libraries(client_a Qt${QT_VERSION_MAJOR}::Core)

add_executable(client_b client_b.cpp shared_data.h)
target_link_libraries(client_b Qt${QT_VERSION_MAJOR}::Core)


include(GNUInstallDirs)
install(TARGETS client_a client_b
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
