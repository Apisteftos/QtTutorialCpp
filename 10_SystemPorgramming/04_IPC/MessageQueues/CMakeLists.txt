cmake_minimum_required(VERSION 3.14)

project(MessageQueues LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

# 1. Create the SENDER executable
add_executable(sender sender.cpp)
target_link_libraries(sender Qt6::Core rt) # Link 'rt' for message queues!

# 2. Create the RECEIVER executable
add_executable(receiver receiver.cpp)
target_link_libraries(receiver Qt6::Core rt) # Link 'rt' for message queues!

# Installation (Optional)
include(GNUInstallDirs)
install(TARGETS sender receiver
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
