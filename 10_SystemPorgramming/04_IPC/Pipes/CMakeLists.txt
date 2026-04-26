cmake_minimum_required(VERSION 3.14)

project(Pipes LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)


add_executable(anonymous_pipe anonymous_pipe.cpp)
target_link_libraries(anonymous_pipe Qt${QT_VERSION_MAJOR}::Core)

add_executable(reader named_pipe_reader.cpp)
target_link_libraries(reader Qt${QT_VERSION_MAJOR}::Core)


add_executable(writer named_pipe_writer.cpp)
target_link_libraries(writer Qt${QT_VERSION_MAJOR}::Core)

include(GNUInstallDirs)
install(TARGETS anonymous_pipe reader writer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
