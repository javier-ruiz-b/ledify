#include "FadeLayer.h"

#include "TimeControl.h"
#include <QtDebug>
#include <cmath>

#define INTERPOLATOR_ACCELERATION_FACTOR 2.0

FadeLayer::FadeLayer() {
    m_source = nullptr;
    m_destination = nullptr;
    m_time = TimeControl::instance();
}

unsigned char FadeLayer::interpolatedDestinationValue() {
    switch (m_interpolator) {
    case InterpolatorAccelerate:
        return interpolatedAcceleratedValue();
    case InterpolatorDecelerate:
        return interpolatedDeceleratedValue();
    case InterpolatorLinear:
        ;
    }
    return static_cast<unsigned char>(
                 static_cast<unsigned long>(m_currentTimeDifferenceMs * 256) /
                 static_cast<unsigned long>(m_durationMs));
}

unsigned char FadeLayer::interpolatedAcceleratedValue() {
    float timeDiffPow = powf(static_cast<float>(m_currentTimeDifferenceMs), INTERPOLATOR_ACCELERATION_FACTOR);
    float durationMsPow = powf(static_cast<float>(m_durationMs), INTERPOLATOR_ACCELERATION_FACTOR);
    return static_cast<unsigned char>((timeDiffPow * 256) / durationMsPow);
}

unsigned char FadeLayer::interpolatedDeceleratedValue() {
    unsigned long timeDiffInv = m_durationMs - m_currentTimeDifferenceMs;
    float durationMsPow = powf(static_cast<float>(m_durationMs), INTERPOLATOR_ACCELERATION_FACTOR);
    float timeDiffPow = powf(static_cast<float>(timeDiffInv), INTERPOLATOR_ACCELERATION_FACTOR);
    float timeDiffPowInv = durationMsPow - timeDiffPow;
    return static_cast<unsigned char>((timeDiffPowInv * 256) / durationMsPow);
}

void FadeLayer::setParams(Layer *source, Layer *destination, FadeLayer::Interpolator interpolator, uint16_t startTimeMs, uint16_t durationMs) {
    m_source = source;
    m_destination = destination;
    m_interpolator = interpolator;
    m_startMs = m_time->millis() + startTimeMs;
    m_durationMs = durationMs;
    m_source->setParent(this);
    m_destination->setParent(this);
}

void FadeLayer::recalculateTimeDifference() {
    unsigned long currentTimeMs = m_time->millis();
    if (currentTimeMs < m_startMs) { // animation didn't begin
        m_currentTimeDifferenceMs = 0;
    } else {
        m_currentTimeDifferenceMs = static_cast<uint16_t>(currentTimeMs - m_startMs);
    }
}

void FadeLayer::startDraw() {
    m_source->startDraw();
    m_destination->startDraw();
    recalculateTimeDifference();
    if (finished()) {
        qDebug() << "FadeLayer"
                 << static_cast<void *>(this)
                 << "finished. (" << static_cast<unsigned int>(m_currentTimeDifferenceMs)
                 << ">" << static_cast<unsigned int>(m_durationMs)
                 << m_time->millis()
                 << "-" << m_startMs;
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
    qDebug() <<"StartLayer new child"
            << static_cast<void *>(this)
            << static_cast<void *>(currentChild)
            << static_cast<void *>(newChild);
    newChild->setParent(this);
    if (currentChild == m_source) {
        m_source = newChild;
    } else if (newChild == m_destination) {
        m_destination = newChild;
    } else {
        qCritical() << "Couldn't set new child: Unknown current ptr!:" << static_cast<void *>(currentChild);
    }
}

void FadeLayer::setInUse(bool value) {
    qDebug() << "FadeLayer" << static_cast<void *>(this)
             << static_cast<int>(value);
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

uint32_t FadeLayer::pixel(uint16_t index) {
    if (m_currentTimeDifferenceMs >= m_durationMs) {
        return m_destination->pixel(index);
    }

    uint32_t sourcePixel = m_source->pixel(index);
    uint32_t destinationPixel = m_destination->pixel(index);
    unsigned char alphaDestination = interpolatedDestinationValue();
    uint16_t alphaSource = 256 - alphaDestination;

    unsigned char w = (unsigned char) (((uint16_t)(sourcePixel >> 24) * alphaSource
                    + (uint16_t)(destinationPixel >> 24) * alphaDestination) / 256);
    unsigned char r = (unsigned char) (((uint16_t)((sourcePixel >> 16) & 255) * alphaSource
                    + (uint16_t)((destinationPixel >> 16) & 255) * alphaDestination) / 256);
    unsigned char g = (unsigned char) (((uint16_t)((sourcePixel >> 8) & 255) * alphaSource
                    + (uint16_t)((destinationPixel >> 8) & 255) * alphaDestination) / 256);
    unsigned char b = (unsigned char) (((uint16_t)(sourcePixel & 255) * alphaSource
                    + (uint16_t)(destinationPixel & 255) * alphaDestination) / 256);
    return ((uint32_t)w << 24) |
           ((uint32_t)r << 16) |
           ((uint32_t)g << 8) |
            b;
}
