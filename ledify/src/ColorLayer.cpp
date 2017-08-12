#include "ColorLayer.h"

void ColorLayer::setColor(byte w, byte r, byte g, byte b) {
    m_pixel = ((uint32)w << 24) |
            ((uint32)r << 16) |
            ((uint32)g << 8) |
            (uint32)b;
}
