macro(addLibrary libName)
    set (ExtraLibraries ${ARGN})
    add_library (${libName} ${libName}.cpp ${libName}.h)
    target_link_libraries (${libName} ${ExtraLibraries} Qt5::Core)
endmacro()

find_package (Threads)
find_package (Qt5Core)
include_directories (${Qt5Core_INCLUDE_DIRS})

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

# main
add_executable(${PROJECT_NAME}_rpi main_rpi.cpp)
target_link_libraries(${PROJECT_NAME}_rpi rpi_ws281x wiringPi LedStripController SerialPort)
