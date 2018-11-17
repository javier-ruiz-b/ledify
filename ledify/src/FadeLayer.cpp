#include "FadeLayer.h"

#include "TimeControl.h"
#include <QtDebug>
#include <QLoggingCategory>
#include <cmath>

Q_LOGGING_CATEGORY(FADE, "ledify.fade", QtWarningMsg)

FadeLayer::FadeLayer() {
    m_time = TimeControl::instance();
}

unsigned char FadeLayer::interpolatedDestinationValue() {
    auto current = static_cast<float>(m_currentTimeDifferenceMs);
    auto end = static_cast<float>(m_durationMs);
    auto floatValue = Interpolator::value(m_interpolator, current, end);
    if (floatValue < 0 || floatValue > 1)  {
        qCWarning(FADE) << "Interpolator out of bounds [0, 1]:" << floatValue;
    }
    return static_cast<unsigned char>(roundf(floatValue * 255));
}

void FadeLayer::setParams(QSharedPointer<Layer> source, QSharedPointer<Layer> destination, Interpolator::Type interpolator, uint16_t startTimeMs, uint16_t durationMs) {
    m_source = source;
    m_destination = destination;
    m_interpolator = interpolator;
    m_delayFromStart = startTimeMs;
    m_durationMs = durationMs;
    m_source->setParent(this);
    m_destination->setParent(this);
}

void FadeLayer::recalculateTimeDifference() {
    uint32_t currentTimeMs = m_time->millis();
    if (!m_startSet) {
        m_startMs = currentTimeMs + static_cast<uint32_t>(m_delayFromStart);
        m_startSet = true;
    }
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
    m_alphaDestination = interpolatedDestinationValue();
    if (finished()) {
        qCDebug(FADE) << "FadeLayer"
                 << static_cast<void *>(this)
                 << "finished. (" << static_cast<unsigned int>(m_currentTimeDifferenceMs)
                 << ">" << static_cast<unsigned int>(m_durationMs)
                 << m_time->millis()
                 << "-" << m_startMs;
        m_parent->setNewChild(this, m_destination);
    }
}

void FadeLayer::endDraw() {
    m_source->endDraw();
    m_destination->endDraw();
}

void FadeLayer::setNewChild(Layer *currentChild, QSharedPointer<Layer> newChild) {
    qCDebug(FADE) <<"StartLayer new child"
            << static_cast<void *>(this)
            << static_cast<void *>(currentChild)
            << static_cast<void *>(newChild.data());
    newChild->setParent(this);
    if (currentChild == m_source) {
        m_source = newChild;
    } else if (newChild == m_destination) {
        m_destination = newChild;
    } else {
        qCCritical(FADE) << "Couldn't set new child: Unknown current ptr!:" << static_cast<void *>(currentChild);
    }
}

bool FadeLayer::finished() {
    return m_currentTimeDifferenceMs > m_durationMs;
}

inline uint8_t pixelComponent(uint32_t sourcePixel,
                              uint32_t destinationPixel,
                              uint16_t alphaSource,
                              uint8_t alphaDestination,
                              int shift) {
    return (static_cast<uint16_t>((sourcePixel >> shift) & 255) * alphaSource
             + static_cast<uint16_t>((destinationPixel >> shift) & 255) * alphaDestination)
            / 256;
}

uint32_t FadeLayer::pixel(uint16_t index) {
    if (m_currentTimeDifferenceMs >= m_durationMs) {
        return m_destination->pixel(index);
    }

    uint32_t sourcePixel = m_source->pixel(index);
    uint32_t destinationPixel = m_destination->pixel(index);
    uint16_t alphaSource = 256 - m_alphaDestination;

    unsigned char w = pixelComponent(sourcePixel, destinationPixel, alphaSource, m_alphaDestination, 24);
    unsigned char r = pixelComponent(sourcePixel, destinationPixel, alphaSource, m_alphaDestination, 16);
    unsigned char g = pixelComponent(sourcePixel, destinationPixel, alphaSource, m_alphaDestination, 8);
    unsigned char b = pixelComponent(sourcePixel, destinationPixel, alphaSource, m_alphaDestination, 0);

//        unsigned char w = (unsigned char) (((uint16_t)(sourcePixel >> 24) * alphaSource
//                        + (uint16_t)(destinationPixel >> 24) * m_alphaDestination) / 256);
//        unsigned char r = (unsigned char) (((uint16_t)((sourcePixel >> 16) & 255) * alphaSource
//                        + (uint16_t)((destinationPixel >> 16) & 255) * m_alphaDestination) / 256);
//        unsigned char g = (unsigned char) (((uint16_t)((sourcePixel >> 8) & 255) * alphaSource
//                        + (uint16_t)((destinationPixel >> 8) & 255) * m_alphaDestination) / 256);
//        unsigned char b = (unsigned char) (((uint16_t)(sourcePixel & 255) * alphaSource
//                        + (uint16_t)(destinationPixel & 255) * m_alphaDestination) / 256);

    return (static_cast<uint32_t>(w) << 24) |
           (static_cast<uint32_t>(r) << 16) |
           (static_cast<uint32_t>(g) << 8) |
           b;
}
