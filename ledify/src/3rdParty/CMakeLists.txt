# qHttp
#add_definitions(-DQHTTP_HAS_CLIENT)
#aux_source_directory(qhttp/3rdparty/http-parser HTTP_PARSER_SOURCES)
#add_library(qHttp qhttp/src/qhttpabstracts.cpp qhttp/src/qhttpserverconnection.cpp qhttp/src/qhttpserverrequest.cpp qhttp/src/qhttpserverresponse.cpp qhttp/src/qhttpserver.cpp qhttp/src/qhttpclient.cpp qhttp/src/qhttpclientrequest.cpp qhttp/src/qhttpclientresponse.cpp ${HTTP_PARSER_SOURCES})
##include_directories(qhttp/src qhttp/src/private qhttp/3rdparty qhttp/3rdparty/http-parser)

#if(WIN32)
#    target_compile_definitions(qHttp PRIVATE QHTTP_EXPORT)
#endif()
#target_link_libraries(qHttp Qt5::Core Qt5::Network)

# Dusk2Dawn
aux_source_directory(Dusk2Dawn DUSK2DAWN_SOURCES)
add_library(Dusk2Dawn ${DUSK2DAWN_SOURCES})

if(BUILD_LEDIFY)
    aux_source_directory(rpi_ws281x RPI_WS281X_SOURCES)
    list(REMOVE_ITEM RPI_WS281X_SOURCES "rpi_ws281x/main.c")
    add_library(rpi_ws281x ${RPI_WS281X_SOURCES})
    # WiringPi
    #    aux_source_directory(wiringPi/wiringPi WIRING_PI_SOURCES)
    #    include_directories(wiringPi wiringPi/wiringPi)
    #    add_library(wiringPi ${WIRING_PI_SOURCES})
    #    target_link_libraries(wiringPi Threads::Threads)
endif()

