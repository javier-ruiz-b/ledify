cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Wno-gnu-zero-variadic-macro-arguments")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
if(UNIX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")
endif()

#set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -fPIC -Wno-gnu-zero-variadic-macro-arguments")

set(CMAKE_AUTOMOC ON)

project(ledify)
add_subdirectory(src)

option(BUILD_TESTS "Build test programs" OFF)
if(BUILD_TESTS)
    enable_testing()
    find_package(Qt5Test)
    add_subdirectory(test)
endif()

option(QML_DEBUGGING "QML Debug" OFF)
if(QML_DEBUGGING)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DQT_QML_DEBUG")
endif()
