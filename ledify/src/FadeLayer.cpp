#include "FadeLayer.h"

byte FadeLayer::interpolatedDestinationValue(uint32 timeMs) {
    switch (m_interpolator) {
    case LINEAR_INTERPOLATOR:
    default:
        return (byte) (((timeMs - m_startMs) * 256) / m_durationMs);
    }
}

void FadeLayer::setInterpolator(byte interpolatorType) {
    m_interpolator = interpolatorType;
}

uint32 FadeLayer::pixel(uint16 position) {
    uint32 timeMs = millis();
    if (m_startMs == 0) {
        m_startMs = timeMs;
    }

    if (timeMs >= (m_startMs + m_durationMs)) {
        return m_destination->pixel(position);
    }

    uint32 sourcePixel = m_source->pixel(position);
    uint32 destinationPixel = m_destination->pixel(position);
    byte alphaDestination = interpolatedDestinationValue(timeMs);
    uint16 alphaSource = 256 - alphaDestination;

    byte w = (byte) (((uint16)(sourcePixel >> 24) * alphaSource
                    + (uint16)(destinationPixel >> 24) * alphaDestination) / 256);
    byte r = (byte) (((uint16)((sourcePixel >> 16) & 255) * alphaSource
                    + (uint16)((destinationPixel >> 16) & 255) * alphaDestination) / 256);
    byte g = (byte) (((uint16)((sourcePixel >> 8) & 255) * alphaSource
                    + (uint16)((destinationPixel >> 8) & 255) * alphaDestination) / 256);
    byte b = (byte) (((uint16)(sourcePixel & 255) * alphaSource
                    + (uint16)(destinationPixel & 255) * alphaDestination) / 256);
    return ((uint32)w << 24) |
           ((uint32)r << 16) |
           ((uint32)g << 8) |
           b;
}
