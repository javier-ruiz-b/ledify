find_package(Qt5 COMPONENTS Core Test)


macro(addQtTest TestLibrary)
    set (ExtraLibraries ${ARGN})
    set (TestName ${TestLibrary}Test)
    add_executable (${TestName} ${TestName}.cpp ${TestName}.h)
    target_include_directories(${TestName} PRIVATE ../src ../src/layers)
    add_test (NAME ${TestName} COMMAND ${TestName})
    target_link_libraries (${TestName} ${TestLibrary} ${ExtraLibraries} Qt5::Core Qt5::Test)
endmacro()

addQtTest(Daytime)
addQtTest(CommandReader)
addQtTest(ColorLayer Layer)
addQtTest(FadeLayer ColorLayer StartLayer TimeControl Layer)
addQtTest(LedStripController CommandReader FadeLayer ColorLayer StartLayer TimeControl MockLedStrip MockRelayController)

qt5_add_resources(RCC_SOURCES SmokeTestResources.qrc)
add_executable (SmokeTest ${RCC_SOURCES} SmokeTest.cpp SmokeTest.h AlwaysOnRelayController.h SmokeTestResources.qrc)
target_include_directories(SmokeTest PRIVATE ../src ../src/layers)
target_link_libraries (SmokeTest LedStripController MockLedStrip MockRelayController Qt5::Core Qt5::Test)
