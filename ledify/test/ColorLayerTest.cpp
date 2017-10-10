#include "ColorLayerTest.h"
#include <QtTest>
#include "ColorLayer.h"

void ColorLayerTest::checksPixelColor() {
    ColorLayer tested(1, 2, 3, 4);
    QCOMPARE(tested.pixel(0), (uint32)0x04010203);
}

QTEST_APPLESS_MAIN(ColorLayerTest)
