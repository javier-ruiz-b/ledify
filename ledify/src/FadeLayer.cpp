#include "FadeLayer.h"

byte FadeLayer::interpolatedDestinationValue() {
    switch (m_interpolator) {
    case InterpolatorAccelerate:
        return interpolatedAcceleratedValue();
    case InterpolatorDecelerate:
        return interpolatedDeceleratedValue();
    case InterpolatorLinear:
    default:
        return (byte) (((uint32)m_currentTimeDifferenceMs * 256) / (uint32)m_durationMs);
    }
}

byte FadeLayer::interpolatedAcceleratedValue() {
    uint32 timeDiffPow = (uint32)m_currentTimeDifferenceMs * (uint32)m_currentTimeDifferenceMs;
    uint32 durationMsPow = (uint32)m_durationMs * (uint32)m_durationMs;
    return (byte) ((timeDiffPow * 256) / durationMsPow);
}

byte FadeLayer::interpolatedDeceleratedValue() {
    uint32 timeDiffInv = m_durationMs - m_currentTimeDifferenceMs;
    uint32 durationMsPow = (uint32)m_durationMs * (uint32)m_durationMs;
    uint32 timeDiffPow = (uint32)timeDiffInv * (uint32)timeDiffInv;
    uint32 timeDiffPowInv = durationMsPow - timeDiffPow;
    return (byte) ((timeDiffPowInv * 256) / durationMsPow);
}

void FadeLayer::setParams(Layer *source,
                          Layer *destination,
                          FadeLayer::Interpolator interpolator,
                          uint32 startTimeMs,
                          uint16 durationMs) {
    m_source = source;
    m_destination = destination;
    m_interpolator = interpolator;
    m_startMs = startTimeMs;
    m_durationMs = durationMs;
}

void FadeLayer::startPixel() {
    uint32 currentTimeMs = millis();
    if (m_startMs == 0) {
        m_startMs = currentTimeMs;
    }
    m_currentTimeDifferenceMs = (uint16) (currentTimeMs - m_startMs);
}

void FadeLayer::endPixel() {
}

uint32 FadeLayer::pixel(uint16 position) {
    if (m_currentTimeDifferenceMs >= m_durationMs) {
        return m_destination->pixel(position);
    }

    uint32 sourcePixel = m_source->pixel(position);
    uint32 destinationPixel = m_destination->pixel(position);
    byte alphaDestination = interpolatedDestinationValue();
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

bool FadeLayer::finished() {
    return m_currentTimeDifferenceMs > m_durationMs;
}
