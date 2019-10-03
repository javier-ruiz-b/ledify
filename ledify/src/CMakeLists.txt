macro(addLibrary libName)
    set (ExtraLibraries ${ARGN})
    add_library (${libName} ${libName}.cpp ${libName}.h)
    target_link_libraries (${libName} ${ExtraLibraries} Qt5::Core)
endmacro()

find_package (Threads)
find_package(Qt5 COMPONENTS Core Network)

option(BUILD_LEDIFY "Build Ledify" ON)
option(BUILD_TEST_RESTSERVER "Build Test RestServer" OFF)
option(BUILD_SIMULATOR "Build simulator" OFF)

add_definitions(-DCPPHTTPLIB_USE_SELECT)

add_subdirectory(3rdParty)
include_directories(3rdParty)

# Source libraries
include_directories ("." "layers")
addLibrary (CommandReader)
addLibrary (Interpolator)
add_subdirectory(layers)
addLibrary (FpsCalculator)
addLibrary (Daytime Dusk2Dawn)
addLibrary (TimeControl)
addLibrary (IRelayController)
addLibrary (RestClientRelayController IRelayController)
addLibrary (LayerController Layer FadeLayer ColorLayer StartLayer RandomLayer SpotLayer SlideAnimationLayer AdditionLayer)
addLibrary (CommandExecutor LayerController TimeControl Daytime)
addLibrary (RestServer)
addLibrary (LedStripController CommandReader FpsCalculator CommandExecutor RestClientRelayController TimeControl)

if (BUILD_TESTS OR BUILD_SIMULATOR)
    addLibrary (MockRelayController IRelayController)
    addLibrary (MockLedStrip)
#    addLibrary (MockWiringPi)
endif()

# main
if(BUILD_LEDIFY)
#    addLibrary (SerialPort wiringPi)
#    addLibrary (WiringPiWrapper wiringPi)
    include_directories(3rdParty/rpi_ws281x)
    addLibrary (Ws2811LedStrip rpi_ws281x Layer)
    add_executable(${PROJECT_NAME} ledify.cpp)
    target_link_libraries(${PROJECT_NAME} LedStripController RestServer Ws2811LedStrip)
endif()

if(BUILD_TEST_RESTSERVER)
    find_package(Qt5 COMPONENTS Gui Widgets)
    add_executable(TestRestServer TestRestServer.cpp)
    target_link_libraries(TestRestServer RestServer Qt5::Core Qt5::Gui Qt5::Widgets)
endif()

if(BUILD_SIMULATOR)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    find_package(Qt5 COMPONENTS Gui Qml Quick QuickControls2 Widgets)
    add_executable(Simulator Simulator.cpp qml.qrc)

    target_link_libraries(Simulator LedStripController MockLedStrip MockRelayController Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::QuickControls2 Qt5::Widgets)
endif()
