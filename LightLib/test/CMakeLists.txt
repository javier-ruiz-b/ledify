include_directories (${TEST_SOURCE_DIR}/src)

add_executable (CommandReaderTest CommandReaderTest.cpp CommandReaderTest.h)

target_link_libraries (CommandReaderTest
                        CommandReader
                        Qt5::Core
                        Qt5::Test)

target_include_directories(CommandReaderTest PRIVATE ../src)

add_test (NAME CommandReaderTest COMMAND CommandReaderTest)
