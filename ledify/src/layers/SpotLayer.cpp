#include "SpotLayer.h"
#include <Color.h>
#include <QtDebug>

SpotLayer::SpotLayer(const Color &color, float position, float size, Interpolator::Type interpolator) {
    m_position = position;
    uint32_t bufferSize = 2 * static_cast<uint32_t>(size);
    m_bufferSize = bufferSize;

    m_pixelBuffer = new uint32_t[bufferSize];
    memset(m_pixelBuffer, 0, bufferSize * sizeof(uint32_t));
    for (uint32_t i = 0; i < m_bufferSize/2; i++) {
    //TODO:here
        auto value = Interpolator::value(interpolator, size - i, size);
        Color resultingColor(color, value);
        m_pixelBuffer[m_bufferSize/2 - i] = resultingColor.rgbw();
        m_pixelBuffer[m_bufferSize/2 + i] = resultingColor.rgbw();
    }
}

SpotLayer::~SpotLayer() {
    delete[] m_pixelBuffer;
}


void SpotLayer::draw(uint32_t *buffer, uint32_t size) {
    auto start = static_cast<uint32_t>(m_position - m_bufferSize/2);
    auto end = static_cast<uint32_t>(m_position + m_bufferSize/2);

    memset(buffer, 0, start * sizeof(uint32_t));
    memcpy(&buffer[start], m_pixelBuffer, m_bufferSize * sizeof(uint32_t));
    memset(&buffer[end], 0, (size - end) * sizeof(uint32_t));
}
