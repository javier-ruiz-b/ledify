#include "ColorLayerTest.h"
#include <QtTest>
#include "ColorLayer.h"

void ColorLayerTest::checksPixelColor() {
    ColorLayer tested;
    tested.setColor(1, 2, 3, 4);
    QCOMPARE(tested.pixel(0), (uint32)0x01020304);
}

QTEST_APPLESS_MAIN(ColorLayerTest)
