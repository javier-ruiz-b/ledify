#find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
include_directories (${TEST_SOURCE_DIR}/src)
#                     ${Boost_INCLUDE_DIRS}
#                     )
#add_definitions (-DBOOST_TEST_DYN_LINK)
add_executable (SerialParserTest SerialParserTest.cpp SerialParserTest.h)

find_package(Qt5Core)
find_package(Qt5Test)
target_link_libraries (SerialParserTest
                        SerialParser
                        Qt5::Core
                        Qt5::Test)
target_include_directories(SerialParserTest PRIVATE ../src)
#target_link_libraries (Test
#                       Sqr
#                       ${Boost_FILESYSTEM_LIBRARY}
#                       ${Boost_SYSTEM_LIBRARY}
#                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
#                       )
add_test (NAME SerialParserTest COMMAND SerialParserTest)
