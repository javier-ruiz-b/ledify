cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-g -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")
set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "-g -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "-g -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")
set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "-g -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")
#set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

project(ledify)
add_subdirectory(src)

option(BUILD_TESTS "Build test programs" OFF)

if(BUILD_TESTS)
    enable_testing()
    find_package(Qt5Test)
    add_subdirectory(test)
endif()
