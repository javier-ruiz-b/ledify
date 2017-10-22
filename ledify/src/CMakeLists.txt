add_library (CommandReader CommandReader.cpp CommandReader.h)
add_library (definitions definitions.cpp definitions.h)
add_library (Layer Layer.cpp Layer.h)
add_library (ColorLayer ColorLayer.cpp ColorLayer.h)
add_library (FadeLayer FadeLayer.cpp FadeLayer.h)
add_library (StartLayer StartLayer.cpp StartLayer.h)
add_library (FPSCalculator FpsCalculator.cpp FpsCalculator.h)
add_library (LayerController LayerController.cpp LayerController.h)
add_library (LedStripController LedStripController.cpp LedStripController.h)

#raspberry pi
add_library (rpi_ws281x rpi_ws281x/dma.c rpi_ws281x/mailbox.c rpi_ws281x/pcm.c rpi_ws281x/pwm.c rpi_ws281x/rpihw.c rpi_ws281x/ws2811.c)
add_library (SerialPort SerialPort.cpp SerialPort.h)

target_link_libraries(FPSCalculator definitions)
target_link_libraries(Layer definitions)
target_link_libraries(FadeLayer Layer definitions)
target_link_libraries(LayerController Layer)
target_link_libraries(LedStripController CommandReader FPSCalculator FadeLayer ColorLayer StartLayer LayerController)

add_executable(${PROJECT_NAME} main_rpi.cpp)
target_link_libraries(${PROJECT_NAME} rpi_ws281x LedStripController SerialPort)
