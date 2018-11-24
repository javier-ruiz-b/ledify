#include "SpotLayer.h"
#include <Color.h>
#include <QtDebug>

SpotLayer::SpotLayer(const Color &color, float position, float size, Interpolator::Type interpolator) {
    m_position = position;
    uint32_t bufferSize = 2 * static_cast<uint32_t>(size);
    m_bufferSize = bufferSize;

    delete m_pixelBuffer;
    m_pixelBuffer = new uint32_t[bufferSize];
    memset(m_pixelBuffer, 0, bufferSize * sizeof(uint32_t));
    for (uint32_t i = 0; i < m_bufferSize/2; i++) {
    //TODO:here
        auto value = Interpolator::value(interpolator, size - i, size);
        Color resultingColor(color, value);
        m_pixelBuffer[m_bufferSize/2 - i] = resultingColor.pixel();
        m_pixelBuffer[m_bufferSize/2 + i] = resultingColor.pixel();
    }
}

void SpotLayer::startDraw() {}

uint32_t SpotLayer::pixel(uint16_t index) {
    int start = static_cast<int>(m_position - m_bufferSize/2);
    int end = static_cast<int>(m_position + m_bufferSize/2);
    if ((start <= index) && (index < end)) {
        return m_pixelBuffer[index - start];
    }
    return 0;
}


void SpotLayer::draw(uint32_t *buffer, uint32_t size) {
    auto start = static_cast<uint32_t>(m_position - m_bufferSize/2);
    auto end = static_cast<uint32_t>(m_position + m_bufferSize/2);

    memset(buffer, 0, start * sizeof(uint32_t));
    memcpy(&buffer[start], m_pixelBuffer, m_bufferSize * sizeof(uint32_t));
//    buffer[150] = m_pixelBuffer[150];
    memset(&buffer[end], 0, (size - end) * sizeof(uint32_t));
}

void SpotLayer::setNewChild(Layer *, QSharedPointer<Layer> newChild) {
    qCritical() << "Unexpected: ColorLayer receiving child %p" << newChild;
}
