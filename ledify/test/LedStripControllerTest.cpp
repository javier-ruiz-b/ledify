#include "LedStripControllerTest.h"
#include "LedStripController.h"
#include "FakeNeoPixelLib.h"

#include <iostream>

const uint16 c_numLeds = 300;

void LedStripControllerTest::init() {
    m_fakePixelLib = new Adafruit_NeoPixel(c_numLeds);
    m_tested = new LedStripController(m_fakePixelLib);
    setMockedTime(true);
    setMockMillis(0);
}

void LedStripControllerTest::cleanup() {
    setMockedTime(false);
    delete m_tested;
    delete m_fakePixelLib;
}

void LedStripControllerTest::settingColorLayerClearsAvailableLayer() {
    QCOMPARE(m_tested->m_availableLayers[0], static_cast<Layer *>(nullptr));
    writeCommand("C+COLOR=0,255,0,0,0");
    QVERIFY(m_tested->m_availableLayers[0] != nullptr);
    QCOMPARE(m_tested->m_rootLayer, static_cast<Layer *>(nullptr));

    writeCommand("C+SET=0");

    QCOMPARE(m_tested->m_availableLayers[0], static_cast<Layer *>(nullptr));
    QVERIFY(m_tested->m_rootLayer != nullptr);
}

void LedStripControllerTest::setsRedColor() {
    writeCommand("C+COLOR=0,255,0,0,0");
    writeCommand("C+SET=0");

    m_tested->draw();

    QCOMPARE(m_fakePixelLib->getPixelColor(0), static_cast<uint32>(0x00FF0000)); //WRGB
}

void LedStripControllerTest::createsTwoColorsAndSetsSecond() {
    writeCommand("C+COLOR=0,255,0,0,0");
    writeCommand("C+COLOR=1,0,255,0,0");
    writeCommand("C+SET=1");

    m_tested->draw();

    QCOMPARE(m_fakePixelLib->getPixelColor(0), static_cast<uint32>(0x0000FF00)); //WRGB
    QCOMPARE(m_tested->m_availableLayers[1], static_cast<Layer *>(nullptr));
}

void LedStripControllerTest::fadesBetweenTwoColors() {
    writeCommand("C+COLOR=0,255,0,0,0");
    writeCommand("C+COLOR=1,0,255,0,0");
    writeCommand("C+FADE=2,0,1,0,1000,1000");
    writeCommand("C+SET=2");

    setMockMillis(1500); //in the middle of the fade
    m_tested->draw();

    QCOMPARE(m_fakePixelLib->getPixelColor(0), static_cast<uint32>(0x007F7F00)); //WRGB
    QCOMPARE(m_tested->m_availableLayers[0], static_cast<Layer *>(nullptr));
    QCOMPARE(m_tested->m_availableLayers[1], static_cast<Layer *>(nullptr));
    QCOMPARE(m_tested->m_availableLayers[2], static_cast<Layer *>(nullptr));
}

void LedStripControllerTest::twoFadesAtTheSameTime() {
    writeCommand("C+COLOR=0,255,0,0,0");
    writeCommand("C+COLOR=1,0,255,0,0");
    writeCommand("C+COLOR=2,127,127,255,0");
    writeCommand("C+FADE=3,0,1,0,0,1000"); // COLOR3= 0+1 @ 500ms = 127,127,0,0
    writeCommand("C+FADE=4,2,3,0,0,1000"); // COLOR4= 2+3 € 500ms = 127,127,127,0
    writeCommand("C+SET=4");

    setMockMillis(500); //in the middle of the fade
    m_tested->draw();

    QCOMPARE(m_fakePixelLib->getPixelColor(0), static_cast<uint32>(0x007F7F7F)); //WRGB
}

void LedStripControllerTest::writeCommand(std::string command) {
    for (int i = 0; i < command.length(); i++) {
        m_tested->writeChar(command[i]);
    }
    m_tested->writeChar('\n');
}

QTEST_APPLESS_MAIN(LedStripControllerTest)
