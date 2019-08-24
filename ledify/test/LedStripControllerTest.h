#pragma once

#define NUM_LED 100
#include <QTest>

#include "MockLedStrip.h"

class LedStripController;
class Adafruit_NeoPixel;
class Layer;

class LedStripControllerTest : public QObject {
    Q_OBJECT

public:
    LedStripControllerTest() {}

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
    MockLedStrip m_ledStrip{NUM_LED};
    LedStripController *m_tested = nullptr;
    const int m_numLeds = NUM_LED;
    int m_leds[NUM_LED];
};
