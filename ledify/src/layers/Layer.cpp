#include "Layer.h"
#include <QScopedPointer>

void Layer::setParent(Layer *parent) {
    m_parent = parent;
}

uint32_t Layer::pixel(uint32_t i) {
    uint32_t size = i + 1;
    auto buff = new uint32_t[size];
    draw(buff, size);
    uint32_t rgbw = buff[i];
    delete[] buff;
    return rgbw;
}
