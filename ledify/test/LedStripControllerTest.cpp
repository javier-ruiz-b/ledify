#include "LedStripControllerTest.h"
#include "LedStripController.h"

#include <iostream>

void LedStripControllerTest::init() {
    tempus::setMockedTime(true);
    tempus::setMockMillis(0);
    m_tested = new LedStripController();
//    m_buffer = (int *) m_buffer;
    memset(m_leds, 0xFE, NUM_LED*sizeof(uint32));
}

void LedStripControllerTest::cleanup() {
    tempus::setMockedTime(false);
    delete m_tested;
}

void LedStripControllerTest::settingColorLayerClearsAvailableLayer() {
    QCOMPARE(m_tested->m_layerController.m_availableLayers[0], static_cast<Layer *>(nullptr));
    writeCommand("C+COLOR=0,255,0,0,0");
    QVERIFY(m_tested->m_layerController.m_availableLayers[0] != nullptr);
    QCOMPARE(m_tested->m_layerController.m_rootLayer.child(), static_cast<Layer *>(nullptr));

    writeCommand("C+SET=0");

    QCOMPARE(m_tested->m_layerController.m_availableLayers[0], static_cast<Layer *>(nullptr));
    QVERIFY(m_tested->m_layerController.m_rootLayer.child() != nullptr);
}

void LedStripControllerTest::setsRedColor() {
    writeCommand("C+COLOR=0,255,0,0,0");
    writeCommand("C+SET=0");

    m_tested->draw(reinterpret_cast<uint32 *>(m_leds), NUM_LED);

    QCOMPARE(m_leds[0], 0x00FF0000); //WRGB
}

void LedStripControllerTest::createsTwoColorsAndSetsSecond() {
    writeCommand("C+COLOR=0,255,0,0,0");
    writeCommand("C+SET=0");
    writeCommand("C+COLOR=0,0,255,0,0");
    writeCommand("C+SET=0");

    m_tested->draw(reinterpret_cast<uint32 *>(m_leds), NUM_LED);

    QCOMPARE(m_leds[0], 0x0000FF00); //WRGB
    QCOMPARE(m_tested->m_layerController.m_availableLayers[1], static_cast<Layer *>(nullptr));
}

void LedStripControllerTest::fadesBetweenTwoColors() {
    writeCommand("C+COLOR=0,255,0,0,0");
    writeCommand("C+COLOR=1,0,255,0,0");
    writeCommand("C+FADE=2,0,1,0,1000,1000");
    writeCommand("C+SET=2");

    tempus::setMockMillis(1500); //in the middle of the fade
    m_tested->draw(reinterpret_cast<uint32 *>(m_leds), NUM_LED);

    QCOMPARE(m_leds[0], 0x007F7F00); //WRGB
    QCOMPARE(m_tested->m_layerController.m_availableLayers[0], static_cast<Layer *>(nullptr));
    QCOMPARE(m_tested->m_layerController.m_availableLayers[1], static_cast<Layer *>(nullptr));
    QCOMPARE(m_tested->m_layerController.m_availableLayers[2], static_cast<Layer *>(nullptr));
}

void LedStripControllerTest::twoFadesAtTheSameTime() {
    writeCommand("C+COLOR=0,255,0,0,0");
    writeCommand("C+COLOR=1,0,255,0,0");
    writeCommand("C+COLOR=2,127,127,255,0");
    writeCommand("C+FADE=3,0,1,0,0,1000"); // COLOR3= 0+1 @ 500ms = 127,127,0,0
    writeCommand("C+FADE=4,2,3,0,0,1000"); // COLOR4= 2+3 € 500ms = 127,127,127,0
    writeCommand("C+SET=4");

    tempus::setMockMillis(500); //in the middle of the fade
    m_tested->draw(reinterpret_cast<uint32 *>(m_leds), NUM_LED);

    QCOMPARE(m_leds[0], 0x007F7F7F); //WRGB
}

void LedStripControllerTest::triesToSetAnUnsetIndex() {
    writeCommand("C+SET=0");
    QCOMPARE(m_tested->m_layerController.m_rootLayer.child(), static_cast<Layer *>(nullptr));
}

void LedStripControllerTest::triesToSetAnInvalidIndex() {
    writeCommand("C+SET=65240");
    QCOMPARE(m_tested->m_layerController.m_rootLayer.child(), static_cast<Layer *>(nullptr));
}

void LedStripControllerTest::acceptanceTest() {
    writeCommand("C+COLOR=0,255,0,0,255");
    writeCommand("C+COLOR=1,0,255,255,0");
    writeCommand("C+FADE=4,0,1,1,2000,30000");
    writeCommand("C+COLOR=0,0,255,255,255");
    writeCommand("C+FADE=5,4,0,1,2000,10000");
    writeCommand("C+COLOR=0,0,255,0,0");
    writeCommand("C+FADE=6,5,0,1,8000,29000");
    writeCommand("C+SET=6");
    writeCommand("C+FPS=0");

    FadeLayer *fade = static_cast<FadeLayer *>(m_tested->m_layerController.m_rootLayer.child());
    ColorLayer *sourceColor = static_cast<ColorLayer *>(fade->m_source);
    ColorLayer *destinationColor = static_cast<ColorLayer *>(fade->m_destination);
    QCOMPARE(fade->m_inUse, true);

    for (uint32 timeMs = 0; timeMs < 60*1000; timeMs+=3) {
        tempus::setMockMillis(timeMs); //in the middle of the fade
        m_tested->draw(reinterpret_cast<uint32 *>(m_leds), NUM_LED);
    }

    QCOMPARE(m_leds[0], 0x0000FF00); //WRGB
    QCOMPARE(fade->m_inUse, false);
    QCOMPARE(sourceColor->m_inUse, false);
    QCOMPARE(destinationColor->m_inUse, true);
    QCOMPARE(m_tested->m_layerController.m_rootLayer.child(), static_cast<Layer *>(destinationColor));
}

void LedStripControllerTest::writeCommand(std::string command) {
    for (size_t i = 0; i < command.length(); i++) {
        m_tested->writeChar(command[i]);
    }
    m_tested->writeChar('\n');
}

QTEST_APPLESS_MAIN(LedStripControllerTest)
