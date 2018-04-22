macro(addLibrary libName)
    set (ExtraLibraries ${ARGN})
    add_library (${libName} ${libName}.cpp ${libName}.h)
    target_link_libraries (${libName} ${ExtraLibraries} Qt5::Core)
endmacro()

find_package (Threads)
find_package (Qt5Core)
find_package (Qt5Network)
include_directories (${Qt5Core_INCLUDE_DIRS})

# qHttp
aux_source_directory(qhttp/3rdparty/http-parser HTTP_PARSER_SOURCES)
include_directories(qhttp/src qhttp/src/private qhttp/3rdparty)
add_library(qHttp qhttp/src/qhttpabstracts.cpp qhttp/src/qhttpserverconnection.cpp qhttp/src/qhttpserverrequest.cpp qhttp/src/qhttpserverresponse.cpp qhttp/src/qhttpserver.cpp ${HTTP_PARSER_SOURCES})
target_link_libraries(qHttp Qt5::Core Qt5::Network)

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

# Source libraries
addLibrary (CommandReader)
addLibrary (Layer)
addLibrary (StartLayer)
addLibrary (ColorLayer)
addLibrary (FadeLayer Layer)
addLibrary (FpsCalculator)
addLibrary (TimeControl wiringPi)
addLibrary (LayerController Layer FadeLayer ColorLayer StartLayer)
addLibrary (LedStripController CommandReader FpsCalculator LayerController TimeControl)
addLibrary (SerialPort wiringPi)
addLibrary (RestServer qHttp)

add_executable(TestRestServer TestRestServer.cpp)
target_link_libraries(TestRestServer RestServer)

# main
add_executable(${PROJECT_NAME} ledify.cpp)
target_link_libraries(${PROJECT_NAME} rpi_ws281x wiringPi LedStripController SerialPort RestServer)
