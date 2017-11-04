#include "FadeLayerTest.h"

#include <QtTest>
#include "StartLayer.h"
#include "FadeLayer.h"
#include "ColorLayer.h"

void FadeLayerTest::init() {
    m_startLayer = new StartLayer();
    m_whiteColor = new ColorLayer();
    m_whiteColor->setColor(255, 255, 255, 255);
    m_blackColor = new ColorLayer();
    m_blackColor->setColor(0, 0, 0, 0);
    setMockedTime(true);
    setMockMillis(0);
}

void FadeLayerTest::cleanup() {
    setMockedTime(false);
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
    setMockMillis(5000);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), (uint32)0xFFFFFFFF);
    delete m_tested;
}

void FadeLayerTest::showsDestinationLayer() {
    createFadeLayer();
    m_tested->setParams(m_whiteColor, m_blackColor, FadeLayer::InterpolatorLinear, 5000, 1000);
    setMockMillis(5999);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), (uint32)0x00000000);
    delete m_tested;
}

void FadeLayerTest::finishesAndReplacesStartLayerChild() {
    createFadeLayer();
    m_tested->setParams(m_whiteColor, m_blackColor, FadeLayer::InterpolatorLinear, 5000, 1000);
    setMockMillis(6001);
    m_tested->startDraw();
    QCOMPARE(m_tested->finished(), true);
    QCOMPARE(m_tested->isInUse(), false);
    QCOMPARE(m_startLayer->pixel(0), (uint32)0x00000000);
    delete m_tested;
}

void FadeLayerTest::calculatesLinearFadeMiddleValue() {
    createFadeLayer();
    m_tested->setParams(m_whiteColor, m_blackColor, FadeLayer::InterpolatorLinear, 5000, 1000);
    setMockMillis(5500);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), (uint32)0x7F7F7F7F);
    delete m_tested;
}

void FadeLayerTest::calculatesAcceleratedFadeMiddleValue() {
    createFadeLayer();
    m_tested->setParams(m_whiteColor, m_blackColor, FadeLayer::InterpolatorAccelerate, 5000, 1000);
    setMockMillis(5500);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), (uint32)0xBFBFBFBF);
    delete m_tested;
}

void FadeLayerTest::calculatesDeceleratedFadeMiddleValue() {
    createFadeLayer();
    m_tested->setParams(m_whiteColor, m_blackColor, FadeLayer::InterpolatorDecelerate, 5000, 1000);
    setMockMillis(5500);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), (uint32)0x3F3F3F3F);
    delete m_tested;
}

QTEST_APPLESS_MAIN(FadeLayerTest)
