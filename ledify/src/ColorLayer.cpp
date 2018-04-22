#include "ColorLayer.h"
#include <QtDebug>

ColorLayer::ColorLayer() {
}

void ColorLayer::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char w) {
    m_pixel = ((uint32_t)w << 24) |
            ((uint32_t)r << 16) |
            ((uint32_t)g << 8) |
            (uint32_t)b;
}

uint32_t ColorLayer::pixel(uint16_t) {
    return m_pixel;
}

void ColorLayer::setNewChild(Layer *, Layer *newChild) {
    qCritical() << "Unexpected: ColorLayer receiving child %p" << newChild;
}

void ColorLayer::setInUse(bool value) {
    qDebug() << "ColorLayer " <<  static_cast<void *>(this) <<  static_cast<int>(value);
    m_inUse = value;
}
