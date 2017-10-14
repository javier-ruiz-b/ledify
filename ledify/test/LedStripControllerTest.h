#pragma once

#include <QtTest>

#define NUM_LED 100

class LedStripController;
class Adafruit_NeoPixel;
class LedStripControllerTest : public QObject {
    Q_OBJECT

public:
    CommandReaderTest() {}

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
    const int m_numLeds = NUM_LED;
    int m_leds[NUM_LED];
    char *m_buffer;
    LedStripController *m_tested;
};
