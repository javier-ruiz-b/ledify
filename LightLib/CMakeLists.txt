cmake_minimum_required(VERSION 2.8.12)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

project(LightLib)
add_subdirectory(src)

enable_testing()
add_subdirectory(test)
