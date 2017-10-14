#include "ColorLayer.h"

ColorLayer::ColorLayer() {
}

void ColorLayer::setColor(byte r, byte g, byte b, byte w) {
    m_pixel = ((uint32)w << 24) |
            ((uint32)r << 16) |
            ((uint32)g << 8) |
            (uint32)b;
}

uint32 ColorLayer::pixel(uint16) {
    return m_pixel;
}

void ColorLayer::setInUse(bool value) {
    m_inUse = value;
}
