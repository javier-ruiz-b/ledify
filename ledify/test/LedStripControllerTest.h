#pragma once

#include <QtTest>

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


private:
    void writeCommand(std::string command);

private:
    Adafruit_NeoPixel *m_fakePixelLib;
    LedStripController *m_tested;
};
