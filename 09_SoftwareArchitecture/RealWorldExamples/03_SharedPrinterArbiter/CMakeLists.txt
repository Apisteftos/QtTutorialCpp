cmake_minimum_required(VERSION 3.14)

project(03_SharedPrinterArbiter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(03_SharedPrinterArbiter
  main.cpp
)

include(GNUInstallDirs)
install(TARGETS 03_SharedPrinterArbiter
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
