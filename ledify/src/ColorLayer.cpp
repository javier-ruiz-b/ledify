#include "ColorLayer.h"

ColorLayer::ColorLayer(byte r, byte g, byte b, byte w) {
    setColor(r, g, b, w);
}

void ColorLayer::setColor(byte r, byte g, byte b, byte w) {
    m_pixel = ((uint32)w << 24) |
            ((uint32)r << 16) |
            ((uint32)g << 8) |
            (uint32)b;
}
