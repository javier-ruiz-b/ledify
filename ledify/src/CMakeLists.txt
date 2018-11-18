macro(addLibrary libName)
    set (ExtraLibraries ${ARGN})
    add_library (${libName} ${libName}.cpp ${libName}.h)
    target_link_libraries (${libName} ${ExtraLibraries} Qt5::Core)
endmacro()

find_package (Threads)
find_package(Qt5 COMPONENTS Core Network)

# qHttp
aux_source_directory(qhttp/3rdparty/http-parser HTTP_PARSER_SOURCES)
include_directories(qhttp/src qhttp/src/private qhttp/3rdparty qhttp/3rdparty/http-parser)
add_library(qHttp qhttp/src/qhttpabstracts.cpp qhttp/src/qhttpserverconnection.cpp qhttp/src/qhttpserverrequest.cpp qhttp/src/qhttpserverresponse.cpp qhttp/src/qhttpserver.cpp ${HTTP_PARSER_SOURCES})
if(WIN32)
    target_compile_definitions(qHttp PRIVATE QHTTP_EXPORT)
endif()
target_link_libraries(qHttp Qt5::Core Qt5::Network)

# Dusk2Dawn
aux_source_directory(Dusk2Dawn DUSK2DAWN_SOURCES)
include_directories(Dusk2Dawn)
add_library(Dusk2Dawn ${DUSK2DAWN_SOURCES})

# Source libraries
include_directories ("." "layers")
addLibrary (CommandReader)
addLibrary (Interpolator)
add_subdirectory(layers)
addLibrary (FpsCalculator)
addLibrary (Daytime Dusk2Dawn)
addLibrary (TimeControl)
addLibrary (RelayController)
addLibrary (LayerController Layer FadeLayer ColorLayer StartLayer RandomLayer)
addLibrary (CommandExecutor LayerController TimeControl Daytime)
addLibrary (RestServer qHttp)
addLibrary (LedStripController CommandReader FpsCalculator CommandExecutor RelayController TimeControl)

addLibrary (MockLedStrip)
addLibrary (MockWiringPi)

# main
option(BUILD_LEDIFY "Build Ledify" ON)
if(BUILD_LEDIFY)    
    # WiringPi
    aux_source_directory(wiringPi/wiringPi WIRING_PI_SOURCES)
    include_directories(wiringPi wiringPi/wiringPi)
    add_library(wiringPi ${WIRING_PI_SOURCES})
    target_link_libraries(wiringPi Threads::Threads)

    # rpi_ws281x
    aux_source_directory(rpi_ws281x RPI_WS281X_SOURCES)
    list(REMOVE_ITEM RPI_WS281X_SOURCES "rpi_ws281x/main.c")
    include_directories(rpi_ws281x)
    add_library(rpi_ws281x ${RPI_WS281X_SOURCES})


    addLibrary (SerialPort wiringPi)
    addLibrary (WiringPiWrapper wiringPi)
    addLibrary (Ws2811LedStrip rpi_ws281x Layer)
    add_executable(${PROJECT_NAME} ledify.cpp)
    target_link_libraries(${PROJECT_NAME} LedStripController SerialPort RestServer WiringPiWrapper Ws2811LedStrip)
endif()

option(BUILD_TEST_RESTSERVER "Build Test RestServer" OFF)
if(BUILD_TEST_RESTSERVER)
    find_package(Qt5 COMPONENTS Gui Widgets)
    add_executable(TestRestServer TestRestServer.cpp)
    target_link_libraries(TestRestServer RestServer Qt5::Core Qt5::Gui Qt5::Widgets)
endif()

option(BUILD_SIMULATOR "Build simulator" OFF)
if(BUILD_SIMULATOR)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    find_package(Qt5 COMPONENTS Gui Qml Quick QuickControls2 Widgets)
    add_executable(Simulator Simulator.cpp qml.qrc)
    target_link_libraries(Simulator LedStripController MockLedStrip MockWiringPi Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::QuickControls2 Qt5::Widgets)
endif()
