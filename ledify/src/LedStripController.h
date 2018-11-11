#pragma once

#include "FpsCalculator.h"
#include "CommandReader.h"
#include "CommandExecutor.h"
#include "LayerController.h"
#include "RelayController.h"
#include "RestServer.h"
#include <QObject>
#include "rpi_ws281x/ws2811.h"

class QString;
class Layer;

class LedStripController : public QObject {
    Q_OBJECT

public:
    LedStripController(QObject *parent = nullptr);

    void initializeDependencies();
    LayerController &layerController();
    bool writeChar(char c);
    bool animationFinished();
    QString parseReceivedString(const QString &string);

    void commandOnIfNight();
    void startDrawLoop();
    void terminate();
    void turnOnRelayAndRefresh();

signals:
    void drawPixels(Layer *rootLayer);
    void terminated();

private:
    void initializeLedStrip();
    void deinitialize();
    void deinitializeLedStrip();
    void drawLoop();
    void draw();
    void drawToLedStrip(Layer *rootLayer);
    bool isAnyLedOn();
    void commandOff();

private:
    const int c_trafoPowerOnDelayMs = 500;
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

    ws2811_t m_ledStrip;
    uint32_t *m_ledBuffer;

    friend class LedStripControllerTest;
};
