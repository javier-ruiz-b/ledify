#pragma once

#include "definitions.h"
#include "FpsCalculator.h"
#include "CommandReader.h"
#include "LayerController.h"
#include "StartLayer.h"

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
    LedStripController();

    CommandReader &commandReader();
    void writeChar(char c);
    void draw(char *ledsRgbw, int numLeds);

private:
    void parseCommand();

    void commandSet(const char *command, byte lengthCommand);
    void commandColor(byte lengthCommand, const char *command);
    void commandFade(const char *command, byte lengthCommand);
    void commandFps(byte lengthCommand, const char *command);

    /**
     * @return length of startsWithString if matches
     */
    byte startsWith(const char *string, const char *startsWithString);

private:
    Layer *m_availableLayers[AVAILABLE_LAYERS_NUM];
    StartLayer m_rootLayer;
    FpsCalculator m_fpsCalculator;
    CommandReader m_commandReader;
    LayerController m_layerController;

    friend class LedStripControllerTest;
};
