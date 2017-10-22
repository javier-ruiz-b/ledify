cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)

project(ledify)
add_subdirectory(src)

enable_testing()
find_package(Qt5Core)
find_package(Qt5Test)
add_subdirectory(test)
