include_directories (${TEST_SOURCE_DIR}/src)

macro(addQtTest TestLibrary)
    set(TestName ${TestLibrary}Test)
    add_executable (${TestName} ${TestName}.cpp ${TestName}.h)
    target_link_libraries (${TestName} ${TestLibrary} Qt5::Core Qt5::Test)
    target_include_directories(${TestName} PRIVATE ../src)
    add_test (NAME ${TestName} COMMAND ${TestName})
endmacro()

addQtTest(CommandReader)
addQtTest(ColorLayer)
addQtTest(ColorConversion)
