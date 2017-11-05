add_library (CommandReader CommandReader.cpp CommandReader.h)
add_library (definitions definitions.cpp definitions.h)
add_library (Layer Layer.cpp Layer.h)
add_library (ColorLayer ColorLayer.cpp ColorLayer.h)
add_library (FadeLayer FadeLayer.cpp FadeLayer.h)
add_library (StartLayer StartLayer.cpp StartLayer.h)
add_library (FPSCalculator FpsCalculator.cpp FpsCalculator.h)
add_library (LayerController LayerController.cpp LayerController.h)
add_library (LedStripController LedStripController.cpp LedStripController.h)

target_link_libraries(FPSCalculator definitions)
target_link_libraries(Layer definitions)
target_link_libraries(FadeLayer Layer definitions)
target_link_libraries(LayerController Layer FadeLayer ColorLayer StartLayer)
target_link_libraries(LedStripController CommandReader FPSCalculator LayerController)

#raspberry pi
if (UNIX)
    aux_source_directory(rpi_ws281x RPI_WS281X_SOURCES)
    list(REMOVE_ITEM RPI_WS281X_SOURCES "rpi_ws281x/main.c")
    include_directories(rpi_ws281x)
    add_library(rpi_ws281x ${RPI_WS281X_SOURCES})

    aux_source_directory(wiringPi/wiringPi WIRING_PI_SOURCES)
    include_directories(wiringPi wiringPi/wiringPi)
    add_library(wiringPi ${WIRING_PI_SOURCES})

    add_library(SerialPort SerialPort.cpp SerialPort.h)

    add_executable(${PROJECT_NAME}_rpi main_rpi.cpp)
    target_link_libraries(${PROJECT_NAME}_rpi rpi_ws281x wiringPi LedStripController SerialPort)
endif (UNIX)
