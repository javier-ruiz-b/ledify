#include "SpotLayer.h"
#include <Color.h>
#include <QtDebug>

SpotLayer::SpotLayer() {}

void SpotLayer::setParams(const Color &color, float position, float size, Interpolator::Type interpolator) {
    m_position = position;
    uint32_t bufferSize = 2 * static_cast<uint32_t>(size);
    m_bufferSize = bufferSize;

    delete m_pixelBuffer;
    m_pixelBuffer = new uint32_t[bufferSize];
    for (uint32_t i = 0; i < m_bufferSize/2; i++) {
    //TODO:here
        auto value = Interpolator::value(interpolator, size - i, size);
        Color resultingColor(color, value);
        m_pixelBuffer[m_bufferSize/2 - i] = resultingColor.pixel();
        m_pixelBuffer[m_bufferSize/2 + i] = resultingColor.pixel();
    }
}

uint32_t SpotLayer::pixel(uint16_t index) {
    int start = static_cast<int>(m_position - m_bufferSize/2);
    int end = static_cast<int>(m_position + m_bufferSize/2);
    if ((start <= index) && (index < end)) {
        return m_pixelBuffer[index - start];
    }
    return 0;
}

void SpotLayer::startDraw() {
}

void SpotLayer::setNewChild(Layer *, QSharedPointer<Layer> newChild) {
    qCritical() << "Unexpected: ColorLayer receiving child %p" << newChild;
}
