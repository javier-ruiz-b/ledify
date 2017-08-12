#include "FadeLayerTest.h"

#include <QtTest>
#include "FadeLayer.h"
#include "ColorLayer.h"

void FadeLayerTest::init() {
    m_whiteColor = new ColorLayer();
    m_whiteColor->setColor(255, 255, 255, 255);
    m_blackColor = new ColorLayer();
    m_blackColor->setColor(0, 0, 0, 0);
    m_tested = new FadeLayer();
    m_tested->setSourceLayer(m_whiteColor);
    m_tested->setDestinationLayer(m_blackColor);
    m_tested->setStartTimeMs(5000);
    m_tested->setDuration(1000);
    setMockedTime(true);
}

void FadeLayerTest::cleanup() {
    setMockedTime(false);
    delete m_tested;
    delete m_whiteColor;
    delete m_blackColor;
}

void FadeLayerTest::showsSourceLayer() {
    m_tested->setStartTimeMs(5000);
    setMockMillis(5000);
    QCOMPARE(m_tested->pixel(0), (uint32)0xFFFFFFFF);
}

void FadeLayerTest::showsDestinationLayer() {
    m_tested->setStartTimeMs(5000);
    m_tested->setDuration(1000);
    setMockMillis(5999);
    QCOMPARE(m_tested->pixel(0), (uint32)0x00000000);
}

void FadeLayerTest::calculatesLinearFadeMiddleValue() {
    m_tested->setStartTimeMs(5000);
    m_tested->setDuration(1000);
    setMockMillis(5500);
    QCOMPARE(m_tested->pixel(0), (uint32)0x7F7F7F7F);
}

void FadeLayerTest::calculatesAcceleratedFadeMiddleValue() {
    m_tested->setStartTimeMs(5000);
    m_tested->setDuration(1000);
    m_tested->setInterpolator(2);
    setMockMillis(5500);
    QCOMPARE(m_tested->pixel(0), (uint32)0x90909090);
}

QTEST_APPLESS_MAIN(FadeLayerTest)
