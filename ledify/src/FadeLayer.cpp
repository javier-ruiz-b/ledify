#include "FadeLayer.h"

#include <math.h>
#define INTERPOLATOR_ACCELERATION_FACTOR 2.0

byte FadeLayer::interpolatedDestinationValue() {
    switch (m_interpolator) {
    case InterpolatorAccelerate:
        return interpolatedAcceleratedValue();
    case InterpolatorDecelerate:
        return interpolatedDeceleratedValue();
    case InterpolatorLinear:
    default:
        return (byte) (((unsigned long)m_currentTimeDifferenceMs * 256) / (unsigned long)m_durationMs);
    }
}

byte FadeLayer::interpolatedAcceleratedValue() {
    float timeDiffPow = powf(static_cast<float>(m_currentTimeDifferenceMs), INTERPOLATOR_ACCELERATION_FACTOR);
    float durationMsPow = powf(static_cast<float>(m_durationMs), INTERPOLATOR_ACCELERATION_FACTOR);
    return (byte) ((timeDiffPow * 256) / durationMsPow);
}

byte FadeLayer::interpolatedDeceleratedValue() {
    unsigned long timeDiffInv = m_durationMs - m_currentTimeDifferenceMs;
    float durationMsPow = powf(static_cast<float>(m_durationMs), INTERPOLATOR_ACCELERATION_FACTOR);
    float timeDiffPow = powf(static_cast<float>(timeDiffInv), INTERPOLATOR_ACCELERATION_FACTOR);
    float timeDiffPowInv = durationMsPow - timeDiffPow;
    return (byte) ((timeDiffPowInv * 256) / durationMsPow);
}

FadeLayer::FadeLayer() {
    m_source = nullptr;
    m_destination = nullptr;
}

void FadeLayer::setParams(Layer *source, Layer *destination, FadeLayer::Interpolator interpolator, uint16 startTimeMs, uint16 durationMs) {
    m_source = source;
    m_destination = destination;
    m_interpolator = interpolator;
    m_startMs = millis() + startTimeMs;
    m_durationMs = durationMs;
    m_source->setParent(this);
    m_destination->setParent(this);
}

void FadeLayer::recalculateTimeDifference() {
    unsigned long currentTimeMs = millis();
    if (currentTimeMs < m_startMs) { // animation didn't begin
        m_currentTimeDifferenceMs = 0;
    } else {
        m_currentTimeDifferenceMs = static_cast<uint16>(currentTimeMs - m_startMs);
    }
}

void FadeLayer::startDraw() {
    m_source->startDraw();
    m_destination->startDraw();
    recalculateTimeDifference();
    if (finished()) {
        logdebug("FadeLayer(%p) finished (%u > %u). TimeDiff: %lu - %lu",
                 static_cast<void *>(this),
                 static_cast<unsigned int>(m_currentTimeDifferenceMs),
                 static_cast<unsigned int>(m_durationMs),
                 millis(),
                 m_startMs);
        m_parent->setNewChild(this, m_destination);
        m_destination = nullptr;
        setInUse(false);
    }
}

void FadeLayer::endDraw() {
    m_source->endDraw();
    m_destination->endDraw();
}

void FadeLayer::setNewChild(Layer *currentChild, Layer *newChild) {
    logdebug("StartLayer(%p) new child %p->%p", static_cast<void *>(this), static_cast<void *>(currentChild), static_cast<void *>(newChild));
    newChild->setParent(this);
    if (currentChild == m_source) {
        m_source = newChild;
    } else if (newChild == m_destination) {
        m_destination = newChild;
    } else {
        logerr("Couldn't set new child: Unknown current ptr!: %p", static_cast<void *>(currentChild));
    }
}

void FadeLayer::setInUse(bool value) {
    logdebug("FadeLayer(%p) %d", static_cast<void *>(this), static_cast<int>(value));
    m_inUse = value;
    if (!value) {
        if (m_source) {
            m_source->setInUse(false);
        }
        if (m_destination) {
            m_destination->setInUse(false);
        }
        m_source = nullptr;
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
