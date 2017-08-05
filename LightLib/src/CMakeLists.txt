add_library (SerialParser SerialParser.cpp SerialParser.h)
#add_executable (demo main.cpp)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} SerialParser) # Qt5::Core)

#target_link_libraries (demo Sqr)
