#include "ColorLayerTest.h"
#include <QtTest>
#include <ColorLayer.h>
#include <Color.h>

void ColorLayerTest::checksPixelColor() {
    ColorLayer tested;
    tested.setColor(Color(1, 2, 3, 4));
    QCOMPARE(tested.pixel(0), (uint32_t)0x04010203);
}

QTEST_APPLESS_MAIN(ColorLayerTest)
