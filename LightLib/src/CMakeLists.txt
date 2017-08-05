add_library (CommandReader CommandReader.cpp CommandReader.h)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} CommandReader)
