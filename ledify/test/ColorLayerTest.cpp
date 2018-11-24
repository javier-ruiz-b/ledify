#include "ColorLayerTest.h"
#include <QtTest>
#include <ColorLayer.h>
#include <Color.h>

void ColorLayerTest::checksPixelColor() {
    ColorLayer tested(Color(1, 2, 3, 4));
    QCOMPARE(static_cast<uint32_t>(tested.pixel(0)),
             static_cast<uint32_t>(0x04010203));
}

QTEST_APPLESS_MAIN(ColorLayerTest)
