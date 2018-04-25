#pragma once

#include "FpsCalculator.h"
#include "CommandReader.h"
#include "LayerController.h"
#include "TimeControl.h"

class QString;
class Adafruit_NeoPixel;
class Layer;


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

    LayerController &layerController();
    CommandReader &commandReader();
    bool writeChar(char c);
    bool writeString(const QString &command);
    void draw(uint32_t *ledsRgbw, int numLeds);


private:
    bool parseCommand();

    void commandSet(const char *command, unsigned char lengthCommand);
    void commandColor(unsigned char lengthCommand, const char *command);
    void commandFade(const char *command, unsigned char lengthCommand);
    void commandFadeTo(const char *command, unsigned char lengthCommand);
    void commandFps(unsigned char lengthCommand, const char *command);

    /**
     * @return length of startsWithString if matches
     */
    unsigned char startsWith(const char *string, const char *startsWithString);

private:
    const int c_relayGpioPin = 29;
    FpsCalculator m_fpsCalculator;
    CommandReader m_commandReader;
    LayerController m_layerController;
    TimeControl m_time;

    friend class LedStripControllerTest;
    void commandOff();
    void commandOn();
};
