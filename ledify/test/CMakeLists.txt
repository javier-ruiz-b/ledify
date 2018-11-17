#include_directories (${TEST_SOURCE_DIR}/src)

#include_directories(../src/wiringPi ../src/wiringPi/wiringPi)
find_package(Qt5 COMPONENTS Core Test)

macro(addQtTest TestLibrary)
    set (ExtraLibraries ${ARGN})
    set (TestName ${TestLibrary}Test)
    add_executable (${TestName} ${TestName}.cpp ${TestName}.h)
    target_include_directories(${TestName} PRIVATE ../src)
    add_test (NAME ${TestName} COMMAND ${TestName})
    target_link_libraries (${TestName} ${TestLibrary} ${ExtraLibraries} Qt5::Core Qt5::Test)
endmacro()

addQtTest(CommandReader)
addQtTest(ColorLayer)
addQtTest(FadeLayer ColorLayer StartLayer TimeControl)
addQtTest(LedStripController CommandReader FadeLayer ColorLayer StartLayer TimeControl MockLedStrip MockWiringPi)
addQtTest(Daytime)
