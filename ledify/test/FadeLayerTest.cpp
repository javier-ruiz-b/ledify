#include "FadeLayerTest.h"

#include <QtTest>
#include <TimeControl.h>
#include "StartLayer.h"
#include "FadeLayer.h"
#include "ColorLayer.h"

void FadeLayerTest::init() {
    m_startLayer = new StartLayer();
    m_whiteColor = new ColorLayer();
    m_whiteColor->setColor(255, 255, 255, 255);
    m_blackColor = new ColorLayer();
    m_blackColor->setColor(0, 0, 0, 0);

    TimeControl::instance()->setMocked(true);
    TimeControl::instance()->setMillis(0);
}

void FadeLayerTest::cleanup() {
    TimeControl::instance()->setMocked(false);
    delete m_whiteColor;
    delete m_blackColor;
    delete m_startLayer;
}

void FadeLayerTest::createFadeLayer() {
    m_tested = new FadeLayer();
    m_startLayer->setChild(m_tested);
}

void FadeLayerTest::showsSourceLayer() {
    createFadeLayer();
    m_tested->setParams(m_whiteColor, m_blackColor, FadeLayer::InterpolatorLinear, 5000, 1000);
    TimeControl::instance()->setMillis(5000);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), (uint32_t)0xFFFFFFFF);
    delete m_tested;
}

void FadeLayerTest::showsDestinationLayer() {
    createFadeLayer();
    m_tested->setParams(m_whiteColor, m_blackColor, FadeLayer::InterpolatorLinear, 5000, 1000);
    TimeControl::instance()->setMillis(5999);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), (uint32_t)0x00000000);
    delete m_tested;
}

void FadeLayerTest::showsSourceLayerBeforeAnimationStarts() {
    createFadeLayer();
    m_tested->setParams(m_whiteColor, m_blackColor, FadeLayer::InterpolatorLinear, 5000, 1000);
    TimeControl::instance()->setMillis(0);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), (uint32_t)0xFFFFFFFF);
    delete m_tested;
}

void FadeLayerTest::finishesAndReplacesStartLayerChild() {
    createFadeLayer();
    m_tested->setParams(m_whiteColor, m_blackColor, FadeLayer::InterpolatorLinear, 5000, 1000);
    TimeControl::instance()->setMillis(6001);
    m_tested->startDraw();
    QCOMPARE(m_tested->finished(), true);
    QCOMPARE(m_tested->isInUse(), false);
    QCOMPARE(m_startLayer->pixel(0), (uint32_t)0x00000000);
    delete m_tested;
}

void FadeLayerTest::calculatesLinearFadeMiddleValue() {
    createFadeLayer();
    m_tested->setParams(m_whiteColor, m_blackColor, FadeLayer::InterpolatorLinear, 5000, 1000);
    TimeControl::instance()->setMillis(5500);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), (uint32_t)0x7F7F7F7F);
    delete m_tested;
}

void FadeLayerTest::calculatesAcceleratedFadeMiddleValue() {
    createFadeLayer();
    m_tested->setParams(m_whiteColor, m_blackColor, FadeLayer::InterpolatorAccelerate, 5000, 1000);
    TimeControl::instance()->setMillis(5500);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), (uint32_t)0xBFBFBFBF);
    delete m_tested;
}

void FadeLayerTest::calculatesDeceleratedFadeMiddleValue() {
    createFadeLayer();
    m_tested->setParams(m_whiteColor, m_blackColor, FadeLayer::InterpolatorDecelerate, 5000, 1000);
    TimeControl::instance()->setMillis(5500);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), (uint32_t)0x3F3F3F3F);
    delete m_tested;
}

void FadeLayerTest::checksDecelerateFade() {
    createFadeLayer();
    TimeControl::instance()->setMillis(3536540);
    m_blackColor->setColor(255,  0,   0,  0);
    m_whiteColor->setColor(0,  255, 250, 40);
    m_tested->setParams(m_blackColor, m_whiteColor, FadeLayer::InterpolatorDecelerate, 0, 1000);
    unsigned int previousValues[4] = {0, 255 ,0, 0};
    for (unsigned int i = 0; i <= 1000; i++) {
        TimeControl::instance()->setMillis(3536540 + i);
        m_tested->startDraw();
        uint32_t pixel = m_tested->pixel(0);
        unsigned int w = pixel >> 24;
        unsigned int r = (pixel >> 16) & 0xFF;
        unsigned int g = (pixel >> 8) & 0xFF;
        unsigned int b = pixel & 0xFF;
        QVERIFY(w >= previousValues[0]);
        QVERIFY(r <= previousValues[1]);
        QVERIFY(g >= previousValues[2]);
        QVERIFY(b >= previousValues[3]);
        previousValues[0] = w;
        previousValues[1] = r;
        previousValues[2] = g;
        previousValues[3] = b;
    }
    delete m_tested;
}

QTEST_APPLESS_MAIN(FadeLayerTest)
