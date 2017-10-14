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

FadeLayer::~FadeLayer() {
    delete m_source;
    delete m_destination;
}

void FadeLayer::setParams(Layer *source, Layer *destination, FadeLayer::Interpolator interpolator, uint32 startTimeMs, uint16 durationMs) {
    m_source = source;
    m_destination = destination;
    m_interpolator = interpolator;
    m_startMs = startTimeMs;
    m_durationMs = durationMs;
}

void FadeLayer::startDraw() {
    m_source->startDraw();
    m_destination->startDraw();
    uint32 currentTimeMs = millis();
    m_currentTimeDifferenceMs = (uint16) (currentTimeMs - m_startMs);
}

void FadeLayer::endDraw() {
    m_source->endDraw();
    m_destination->endDraw();
}

void FadeLayer::setNewChild(Layer *currentChild, Layer *newChild) {
    if (currentChild == m_source) {
        m_source = newChild;
    } else if (newChild == m_destination) {
        m_destination = newChild;
    } else {
        logerr("Couldn't set new child: Unknown current ptr!", newChild);
    }
}

void FadeLayer::setInUse(bool value) {
    m_inUse = value;
    if (!value) {
        m_source->setInUse(false);
        m_destination->setInUse(false);
        m_source = nullptr;
        m_destination = nullptr;
    }
}

bool FadeLayer::finished() {
    return m_currentTimeDifferenceMs > m_durationMs;
}

uint32 FadeLayer::pixel(uint16 index) {
    if (m_currentTimeDifferenceMs >= m_durationMs) {
        return m_destination->pixel(index);
    }

    uint32 sourcePixel = m_source->pixel(index);
    uint32 destinationPixel = m_destination->pixel(index);
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
