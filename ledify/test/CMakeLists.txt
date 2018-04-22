include_directories (${TEST_SOURCE_DIR}/src)

include_directories(../src/wiringPi ../src/wiringPi/wiringPi)

macro(addQtTest TestLibrary)
    set (ExtraLibraries ${ARGN})
    set (TestName ${TestLibrary}Test)
    add_executable (${TestName} ${TestName}.cpp ${TestName}.h)
    target_link_libraries (${TestName} ${TestLibrary} ${ExtraLibraries} Qt5::Core Qt5::Test)
    target_include_directories(${TestName} PRIVATE ../src)
    add_test (NAME ${TestName} COMMAND ${TestName})
endmacro()

addQtTest(CommandReader)
addQtTest(ColorLayer)
addQtTest(FadeLayer ColorLayer StartLayer TimeControl)
addQtTest(LedStripController CommandReader FadeLayer ColorLayer StartLayer TimeControl)
