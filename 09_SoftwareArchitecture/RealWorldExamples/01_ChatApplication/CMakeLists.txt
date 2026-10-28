cmake_minimum_required(VERSION 3.14)

project(01_ChatApplication LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_ChatApplication
  main.cpp
)

include(GNUInstallDirs)
install(TARGETS 01_ChatApplication
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
