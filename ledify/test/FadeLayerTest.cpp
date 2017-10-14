#include "FadeLayerTest.h"

#include <QtTest>
#include "FadeLayer.h"
#include "ColorLayer.h"

void FadeLayerTest::init() {
    m_whiteColor = new ColorLayer();
    m_whiteColor->setColor(255, 255, 255, 255);
    m_blackColor = new ColorLayer();
    m_blackColor->setColor(0, 0, 0, 0);
    setMockedTime(true);
}

void FadeLayerTest::cleanup() {
    setMockedTime(false);
    delete m_tested;
    delete m_whiteColor;
    delete m_blackColor;
}

void FadeLayerTest::showsSourceLayer() {
    m_tested = new FadeLayer();
    m_tested->setParams(m_whiteColor, m_blackColor, FadeLayer::InterpolatorLinear, 5000, 1000);
    setMockMillis(5000);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), (uint32)0xFFFFFFFF);
}

void FadeLayerTest::showsDestinationLayer() {
    m_tested = new FadeLayer();
    m_tested->setParams(m_whiteColor, m_blackColor, FadeLayer::InterpolatorLinear, 5000, 1000);
    setMockMillis(5999);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), (uint32)0x00000000);
}

void FadeLayerTest::calculatesLinearFadeMiddleValue() {
    m_tested = new FadeLayer();
    m_tested->setParams(m_whiteColor, m_blackColor, FadeLayer::InterpolatorLinear, 5000, 1000);
    setMockMillis(5500);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), (uint32)0x7F7F7F7F);
}

void FadeLayerTest::calculatesAcceleratedFadeMiddleValue() {
    m_tested = new FadeLayer();
    m_tested->setParams(m_whiteColor, m_blackColor, FadeLayer::InterpolatorAccelerate, 5000, 1000);
    setMockMillis(5500);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), (uint32)0xBFBFBFBF);
}

void FadeLayerTest::calculatesDeceleratedFadeMiddleValue() {
    m_tested = new FadeLayer();
    m_tested->setParams(m_whiteColor, m_blackColor, FadeLayer::InterpolatorDecelerate, 5000, 1000);
    setMockMillis(5500);
    m_tested->startDraw();
    QCOMPARE(m_tested->pixel(0), (uint32)0x3F3F3F3F);
}

QTEST_APPLESS_MAIN(FadeLayerTest)
