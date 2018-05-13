#pragma once

#include "FpsCalculator.h"
#include "CommandReader.h"
#include "CommandExecutor.h"
#include "LayerController.h"
#include "RelayController.h"
#include "RestServer.h"
#include <QObject>

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
class LedStripController : public QObject {
    Q_OBJECT

public:
    LedStripController();

    void initialize();
    LayerController &layerController();
    bool writeChar(char c);
    bool animationFinished();
    QString parseReceivedString(const QString &string);

    void commandOnIfNight();
    void startDrawLoop();
    void terminate();

signals:
    void terminated();

private:
    void initializeWiringPi();
    void initializeLedStrip();
    void deinitialize();
    void deinitializeLedStrip();
    void drawLoop();
    void draw(uint32_t *ledsRgbw, int numLeds);
    bool isAnyLedOn();
    void commandOff();

private:
    const int c_trafoPowerOnDelayMs = 1500;
    const int c_trafoIdlePowerOffDelayMs = 6000;
    const int c_drawRefreshIdleMs = 3000;
    const int c_drawRefreshAnimationMs = 1000 / 50;
    RelayController m_relayController;
    FpsCalculator m_fpsCalculator;
    CommandReader m_commandReader;
    LayerController m_layerController;
    RestServer restServer;
    QScopedPointer<CommandExecutor> m_executor;
    QTimer *m_loopTimer;

    friend class LedStripControllerTest;
    void turnOnRelayAndRefresh();
};
