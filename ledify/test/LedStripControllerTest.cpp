#include "LedStripControllerTest.h"
#include <LedStripController.h>

#include <TimeControl.h>
#include <ColorLayer.h>
#include <Color.h>
#include <iostream>
#include <MockRelayController.h>

void LedStripControllerTest::init() {
    TimeControl::instance()->setMocked(true);
    TimeControl::instance()->setMillis(0);
    m_tested = new LedStripController(m_ledStrip.data(), new MockRelayController(this), this);
//    m_buffer = (int *) m_buffer;
    memset(m_leds.data(), 0xFE, NUM_LED*sizeof(uint32_t));
    connect(m_tested, &LedStripController::drawPixels, this, [this] (Layer *rootLayer) {
        rootLayer->draw(m_leds);
    });
}

void LedStripControllerTest::cleanup() {
    TimeControl::instance()->setMocked(false);
    delete m_tested;
}

void LedStripControllerTest::settingColorLayerClearsAvailableLayer() {
    m_tested->m_layerController.root().reset();
    QCOMPARE(m_tested->m_layerController.m_indexedLayers.contains(0), false);
    writeCommand("C+COLOR=0,255,0,0,0");
    QCOMPARE(m_tested->m_layerController.m_indexedLayers.contains(0), true);
    QVERIFY(m_tested->m_layerController.root().child().isNull());

    writeCommand("C+SET=0");

    QCOMPARE(m_tested->m_layerController.m_indexedLayers.contains(0), false);
    QVERIFY(!m_tested->m_layerController.root().child().isNull());
}

void LedStripControllerTest::setsRedColor() {
    writeCommand("C+COLOR=0,255,0,0,0");
    writeCommand("C+SET=0");

    m_tested->draw();

    QCOMPARE(m_leds[0], 0x00FF0000); //WRGB
}

void LedStripControllerTest::createsTwoColorsAndSetsSecond() {
    writeCommand("C+COLOR=0,255,0,0,0");
    writeCommand("C+SET=0");
    writeCommand("C+COLOR=0,0,255,0,0");
    writeCommand("C+SET=0");

    m_tested->draw();

    QCOMPARE(m_leds[0], 0x0000FF00); //WRGB
    QCOMPARE(m_tested->m_layerController.m_indexedLayers.contains(1), false);
}

void LedStripControllerTest::fadesBetweenTwoColors() {
    writeCommand("C+COLOR=0,255,0,0,0");
    writeCommand("C+COLOR=1,0,255,0,0");
    writeCommand("C+FADE=2,0,1,1,1000,1000");
    writeCommand("C+SET=2");

    m_tested->draw();
    TimeControl::instance()->setMillis(1500); //in the middle of the fade
    m_tested->draw();

    QCOMPARE(m_leds[0], 0x007F7F00); //WRGB
    QCOMPARE(m_tested->m_layerController.m_indexedLayers.contains(0), false);
    QCOMPARE(m_tested->m_layerController.m_indexedLayers.contains(1), false);
    QCOMPARE(m_tested->m_layerController.m_indexedLayers.contains(2), false);
}

void LedStripControllerTest::fadesBetweenCurrentLayerAndAnotherColor() {
    writeCommand("C+COLOR=0,255,0,0,0");
    writeCommand("C+SET=0");
    writeCommand("C+COLOR=1,0,255,0,0");
    writeCommand("C+FADETO=2,1,1,1000,1000");
    writeCommand("C+SET=2");

    m_tested->draw();
    TimeControl::instance()->setMillis(1500); //in the middle of the fade
    m_tested->draw();

    QCOMPARE(m_leds[0], 0x007F7F00); //WRGB
}

void LedStripControllerTest::twoFadesAtTheSameTime() {
    writeCommand("C+COLOR=0,255,0,0,0");
    writeCommand("C+COLOR=1,0,255,0,0");
    writeCommand("C+COLOR=2,127,127,255,0");
    writeCommand("C+FADE=3,0,1,1,0,1000"); // COLOR3= 0+1 @ 500ms = 127,127,0,0
    writeCommand("C+FADE=4,2,3,1,0,1000"); // COLOR4= 2+3 € 500ms = 127,127,127,0
    writeCommand("C+SET=4");

    m_tested->draw();
    TimeControl::instance()->setMillis(500); //in the middle of the fade
    m_tested->draw();

    QCOMPARE(m_leds[0], 0x007F7F7F); //WRGB
}

void LedStripControllerTest::triesToSetAnUnsetIndex() {
    QTest::ignoreMessage(QtSystemMsg,QRegularExpression("Doesn't exist"));
    m_tested->m_layerController.root().reset();
    writeCommand("C+SET=0");
    QVERIFY(m_tested->m_layerController.root().child().isNull());
}

void LedStripControllerTest::triesToSetAnInvalidIndex() {
    QTest::ignoreMessage(QtSystemMsg,QRegularExpression("Doesn't exist"));
    m_tested->m_layerController.root().reset();
    writeCommand("C+SET=65240");
    QVERIFY(m_tested->m_layerController.root().child().isNull());
}

void LedStripControllerTest::oneFadeWithLayerControllerInterface() {
    LayerController &layerControl = m_tested->layerController();
    layerControl.addTo(0, new ColorLayer(Color(0, 0, 0, 0)));
    layerControl.addTo(1, new ColorLayer(Color(0, 255, 0, 0)));

    layerControl.setAsRoot(new FadeLayer(layerControl.at(0),
                                         layerControl.at(1),
                                         Interpolator::InterpolatorAccelerate, 1, 100));
    writeCommand("C+FPS=0");

    for (uint32_t timeMs = 0; timeMs < 130; timeMs+=10) {
        TimeControl::instance()->setMillis(timeMs);
        m_tested->draw();
    }

    QCOMPARE(m_leds[0], 0x0000FF00); //WRGB
}

void LedStripControllerTest::copiesLayer() {
    LayerController &layerControl = m_tested->layerController();
    layerControl.addTo(1, new ColorLayer(Color(0, 255, 0, 0)));
    writeCommand("C+COPY=2,1");
    layerControl.setAsRoot(2);
    m_tested->draw();

    QCOMPARE(m_leds[0], 0x0000FF00); //WRGB
}

void LedStripControllerTest::movesLayer() {
    LayerController &layerControl = m_tested->layerController();
    layerControl.addTo(1, new ColorLayer(Color(0, 255, 0, 0)));
    writeCommand("C+MOVE=2,1");
    layerControl.setAsRoot(2);
    m_tested->draw();

    QCOMPARE(m_leds[0], 0x0000FF00); //WRGB
}

void LedStripControllerTest::fadeAcceptanceTest() {
    writeCommand("C+COLOR=0,255,0,0,255");
    writeCommand("C+COLOR=1,0,255,255,0");
    writeCommand("C+FADE=4,0,1,1,2000,30000");
    writeCommand("C+COLOR=0,0,255,255,255");
    writeCommand("C+FADE=5,4,0,1,2000,10000");
    writeCommand("C+COLOR=0,0,255,0,0");
    writeCommand("C+FADE=6,5,0,1,8000,29000");
    writeCommand("C+SET=6");
    writeCommand("C+FPS=0");

    FadeLayer *fade = static_cast<FadeLayer *>(m_tested->m_layerController.root().child().data());
    ColorLayer *destinationColor = static_cast<ColorLayer *>(fade->m_destination.data());

    for (uint32_t timeMs = 0; timeMs < 60*1000; timeMs+=3) {
        TimeControl::instance()->setMillis(timeMs);
        m_tested->draw();
    }

    QCOMPARE(m_leds[0], 0x0000FF00); //WRGB
    QCOMPARE(m_tested->m_layerController.root().child().data(), static_cast<Layer *>(destinationColor));
}

void LedStripControllerTest::recursiveFades() {
    auto &layerControl = m_tested->layerController();
    auto index = layerControl.add(new ColorLayer(Color(2, 4, 5, 255)));
    layerControl.setAsRoot(index);
    m_tested->draw();
    for (int i = 0; i < 20; i++) {
        auto colorLayer = QSharedPointer<ColorLayer>(new ColorLayer(Color(i, 4, 5, 255)));
        index = layerControl.add(new FadeLayer(layerControl.current(),
                                               colorLayer,
                                               Interpolator::InterpolatorAccelerate, 0, 5));
        TimeControl::instance()->setMillis(static_cast<uint32_t>(i));
        m_tested->draw();
    }
}

void LedStripControllerTest::writeCommand(std::string command) {
    for (size_t i = 0; i < command.length(); i++) {
        m_tested->writeChar(command[i]);
    }
    m_tested->writeChar('\n');
}

QTEST_APPLESS_MAIN(LedStripControllerTest)
