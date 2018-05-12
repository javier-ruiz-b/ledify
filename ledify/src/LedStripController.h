#pragma once

#include "FpsCalculator.h"
#include "CommandReader.h"
#include "CommandExecutor.h"
#include "LayerController.h"

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
    QString parseReceivedString(const QString &string);
    void draw(uint32_t *ledsRgbw, int numLeds);

    void commandOff();
    void commandOnIfNight();

private:
    FpsCalculator m_fpsCalculator;
    CommandReader m_commandReader;
    LayerController m_layerController;
    QScopedPointer<CommandExecutor> m_executor;

    friend class LedStripControllerTest;
};
