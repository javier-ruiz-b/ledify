#include "FadeLayerTest.h"

#include <QtTest>
#include <TimeControl.h>
#include "StartLayer.h"
#include "FadeLayer.h"
#include "ColorLayer.h"
#include <Color.h>

void FadeLayerTest::init() {
    m_startLayer = new StartLayer();
    m_whiteColor.reset(new ColorLayer(Color(255, 255, 255, 255)));
    m_blackColor.reset(new ColorLayer(Color(0, 0, 0, 0)));

    TimeControl::instance()->setMocked(true);
    TimeControl::instance()->setMillis(0);
}

void FadeLayerTest::cleanup() {
    TimeControl::instance()->setMocked(false);
    m_whiteColor.clear();
    m_blackColor.clear();
    delete m_startLayer;
}

void FadeLayerTest::createFadeLayer(FadeLayer *layer) {
    m_tested.reset(layer);
    m_startLayer->setChild(m_tested);
}

void FadeLayerTest::showsSourceLayer() {
    createFadeLayer(new FadeLayer(m_whiteColor, m_blackColor, Interpolator::InterpolatorLinear, 5000, 1000));
    TimeControl::instance()->setMillis(5000);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), 0xFFFFFFFF);
}

void FadeLayerTest::showsDestinationLayer() {
    createFadeLayer(new FadeLayer(m_whiteColor, m_blackColor, Interpolator::InterpolatorLinear, 5000, 1000));
    m_tested->startDraw();
    TimeControl::instance()->setMillis(5999);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), 0x00000000U);
}

void FadeLayerTest::showsSourceLayerBeforeAnimationStarts() {
    createFadeLayer(new FadeLayer(m_whiteColor, m_blackColor, Interpolator::InterpolatorLinear, 5000, 1000));
    m_tested->startDraw();
    TimeControl::instance()->setMillis(0);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), 0xFFFFFFFF);
}

void FadeLayerTest::finishesAndReplacesStartLayerChild() {
    createFadeLayer(new FadeLayer(m_whiteColor, m_blackColor, Interpolator::InterpolatorLinear, 5000, 1000));
    m_tested->startDraw();
    TimeControl::instance()->setMillis(6001);
    m_tested->startDraw();
    QCOMPARE(m_tested->finished(), true);
    QCOMPARE(m_startLayer->pixel(0), 0x00000000U);
}

void FadeLayerTest::calculatesLinearFadeMiddleValue() {
    createFadeLayer(new FadeLayer(m_whiteColor, m_blackColor, Interpolator::InterpolatorLinear, 5000, 1000));
    m_tested->startDraw();
    TimeControl::instance()->setMillis(5500);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), 0x7F7F7F7FU);
}

void FadeLayerTest::calculatesAcceleratedFadeMiddleValue() {
    createFadeLayer(new FadeLayer(m_whiteColor, m_blackColor, Interpolator::InterpolatorAccelerate, 5000, 1000));
    m_tested->startDraw();
    TimeControl::instance()->setMillis(5500);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), 0xBFBFBFBFU);
}

void FadeLayerTest::calculatesDeceleratedFadeMiddleValue() {
    createFadeLayer(new FadeLayer(m_whiteColor, m_blackColor, Interpolator::InterpolatorDecelerate, 5000, 1000));
    m_tested->startDraw();
    TimeControl::instance()->setMillis(5500);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), 0x40404040U);
}

void FadeLayerTest::checksDecelerateFade() {
    TimeControl::instance()->setMillis(3536540);
    m_blackColor->setColor(Color(255,  0,   0,  0));
    m_whiteColor->setColor(Color(0,  255, 250, 40));
    createFadeLayer(new FadeLayer(m_blackColor, m_whiteColor, Interpolator::InterpolatorDecelerate, 0, 1000));
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
}

QTEST_APPLESS_MAIN(FadeLayerTest)
