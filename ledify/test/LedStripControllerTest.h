#pragma once

#define NUM_LED 100
#include <QTest>

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
    void oneFadeWithLayerControllerInterface();
    void acceptanceTest();

private:
    void writeCommand(std::string command);

private:
    LedStripController *m_tested;
    const int m_numLeds = NUM_LED;
    int m_leds[NUM_LED];
};
