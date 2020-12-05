#pragma once

#define NUM_LED 100
#include <QTest>
#include <QVector>

#include "MockLedStrip.h"

class LedStripController;
class Adafruit_NeoPixel;
class Layer;

class LedStripControllerTest : public QObject {
    Q_OBJECT

public:
    LedStripControllerTest() {
        Config::createInstance(this)->setLedCount(NUM_LED);
        m_ledStrip.reset(new MockLedStrip());
    }

private slots:
    void init();
    void cleanup();

    void settingColorLayerClearsAvailableLayer();
    void setsRedColor();
    void createsTwoColorsAndSetsSecond();
    void fadesBetweenTwoColors();
    void fadesBetweenCurrentLayerAndAnotherColor();
    void twoFadesAtTheSameTime();
    void triesToSetAnUnsetIndex();
    void triesToSetAnInvalidIndex();
    void oneFadeWithLayerControllerInterface();
    void copiesLayer();
    void movesLayer();
    void fadeAcceptanceTest();
    void recursiveFades();

private:
    void writeCommand(std::string command);

private:
    QSharedPointer<MockLedStrip> m_ledStrip;
    LedStripController *m_tested = nullptr;
    int m_numLeds = NUM_LED;
    QVector<quint32> m_leds{NUM_LED};
};
