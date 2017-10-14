#pragma once

#include <QtTest>
#include "definitions.h"

#define NUM_LED 100

class LedStripController;
class Adafruit_NeoPixel;
class LedStripControllerTest : public QObject {
    Q_OBJECT

public:
    LedStripControllerTest() {
        m_tested = nullptr;
    }

private slots:
    void init();
    void cleanup();

    void settingColorLayerClearsAvailableLayer();
    void setsRedColor();
    void createsTwoColorsAndSetsSecond();
    void fadesBetweenTwoColors();
    void twoFadesAtTheSameTime();
    void triesToSetAnUnsetIndex();
    void triesToSetAnInvalidIndex();
    void acceptanceTest();

private:
    void writeCommand(std::string command);

private:
    LedStripController *m_tested;
    const int m_numLeds = NUM_LED;
    int *m_leds;
    char m_buffer[NUM_LED*sizeof(uint32)];
};
