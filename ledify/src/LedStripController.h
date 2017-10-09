#pragma once

#include "definitions.h"
#include "CommandReader.h"

class Adafruit_NeoPixel;
class Layer;

#define AVAILABLE_LAYERS_NUM   8

/**
 * @brief The LedStripController class
 * Commands:
 *      C+POWEROFF
 *      C+COLOR=myIndex,R,G,B,W
 *      C+FADE=myIndex,fromIndex,toIndex,interpolator,startDelayMs,durationMs
 *      C+SET=index
 *
 * Behavior:
 *      m_availableLayers saves a list of available layers ready to be used.
 *  m_rootLayer will be used as the start point for the rendering.
 *
 *      Main loop:
 *          1. writeChar()
 *              if writeChar(c) == true
 *                  parseCommand
 *          2. draw()
 *
 *
 *      Draw:
 *          for every layer: call start
 *          for every layer:
 *              for every pixel:
 *                  call pixel
 *                  save in m_neoPixelLib
 *          for every layer: call end
 *
 *
 */
class LedStripController {
public:
    LedStripController(Adafruit_NeoPixel *neoPixelLib);

    CommandReader &commandReader();
    void writeChar(char c);
    void draw();

private:
    CommandReader m_commandReader;
    Adafruit_NeoPixel *m_neoPixelLib;
    Layer *m_availableLayers[AVAILABLE_LAYERS_NUM];
    Layer *m_rootLayer;

    friend class LedStripControllerTest;
};
