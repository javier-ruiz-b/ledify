#pragma once

#include "FpsCalculator.h"
#include "CommandReader.h"
#include "CommandExecutor.h"
#include "LayerController.h"
#include <QObject>

class ILedStrip;
class IWiringPi;
class Layer;
class QString;
class IRelayController;

class LedStripController : public QObject {
    Q_OBJECT

public:
    LedStripController(ILedStrip *ledStrip, IRelayController *relayController, QObject *parent = nullptr);

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
    void deinitialize();
    void drawLoop();
    void draw();
    void drawToLedStrip(Layer *rootLayer);
    bool isAnyLedOn();
    void commandOff();

private:
    const int c_trafoPowerOnDelayMs = 500;
    const int c_trafoIdlePowerOffDelayMs = 4000;
    const int c_drawRefreshIdleMs = 4000;

    ILedStrip *m_ledStrip;
    IRelayController *m_relayController;
    FpsCalculator m_fpsCalculator;
    CommandReader m_commandReader;
    LayerController m_layerController;
    QScopedPointer<CommandExecutor> m_executor;
    QTimer *m_loopTimer;

    friend class LedStripControllerTest;
    friend class SmokeTest;
};
