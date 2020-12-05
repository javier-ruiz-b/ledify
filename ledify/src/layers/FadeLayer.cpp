#include "FadeLayer.h"

#include "TimeControl.h"
#include <QtDebug>
#include <QLoggingCategory>
#include <cmath>

Q_LOGGING_CATEGORY(FADE, "ledify.fade", QtWarningMsg)

unsigned char FadeLayer::interpolatedDestinationValue() {
    auto current = static_cast<float>(m_currentTimeDifferenceMs);
    auto end = static_cast<float>(m_durationMs);
    auto floatValue = Interpolator::value(m_interpolator, current, end);
    if (floatValue < 0 || floatValue > 1)  {
        qCWarning(FADE) << "Interpolator out of bounds [0, 1]:" << floatValue;
        floatValue = qMin(1.0f, qMax(0.0f, floatValue));
    }
    return static_cast<unsigned char>(roundf(floatValue * 255));
}

FadeLayer::FadeLayer(QSharedPointer<Layer> source, QSharedPointer<Layer> destination, Interpolator::Type interpolator, uint16_t startTimeMs, uint16_t durationMs) {
    m_time = TimeControl::instance();
    m_source = source;
    m_destination = destination;
    m_interpolator = interpolator;
    m_delayFromStart = startTimeMs;
    m_durationMs = durationMs;
    m_source->setParent(this);
    m_destination->setParent(this);
}

FadeLayer::~FadeLayer() {}

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

inline uint8_t pixelComponent(uint32_t sourcePixel,
                              uint32_t destinationPixel,
                              uint16_t alphaSource,
                              uint8_t alphaDestination,
                              int shift) {
    return (static_cast<uint16_t>((sourcePixel >> shift) & 255) * alphaSource
           + static_cast<uint16_t>((destinationPixel >> shift) & 255) * alphaDestination)
           / 256;
}

inline uint32_t FadeLayer::drawPixel(uint32_t sourcePixel,
                                     uint32_t destinationPixel,
                                     uint16_t alphaSource,
                                     uint8_t alphaDestination) {
    uint8_t w = pixelComponent(sourcePixel, destinationPixel, alphaSource, alphaDestination, 24);
    uint8_t r = pixelComponent(sourcePixel, destinationPixel, alphaSource, alphaDestination, 16);
    uint8_t g = pixelComponent(sourcePixel, destinationPixel, alphaSource, alphaDestination, 8);
    uint8_t b = pixelComponent(sourcePixel, destinationPixel, alphaSource, alphaDestination, 0);

    return (static_cast<uint32_t>(w) << 24) |
           (static_cast<uint32_t>(r) << 16) |
           (static_cast<uint32_t>(g) << 8) |
           b;
}

void FadeLayer::draw(QVector<quint32> &buffer) {
    recalculateTimeDifference();

    if (m_tempBuffer.isEmpty()) {
        m_tempBuffer.resize(buffer.size());
    }
    auto &sourceBuffer = buffer;
    auto &destinationBuffer = m_tempBuffer;

    m_source->draw(sourceBuffer);
    m_destination->draw(destinationBuffer);

    if (!started()) {
        return;
    }
    if (finished()) {
        qCDebug(FADE) << "FadeLayer"
                 << static_cast<void *>(this)
                 << "finished. (" << static_cast<unsigned int>(m_currentTimeDifferenceMs)
                 << ">" << static_cast<unsigned int>(m_durationMs)
                 << m_time->millis()
                 << "-" << m_startMs;
        m_destination->draw(buffer);
        m_parent->setNewChild(this, m_destination);
        return;
    }

    auto alphaDestination = interpolatedDestinationValue();
    uint16_t alphaSource = 256 - alphaDestination;

    for (int i = 0; i < buffer.size(); i++) {
        buffer[i] = drawPixel(sourceBuffer[i],
                              destinationBuffer[i],
                              alphaSource,
                              alphaDestination);
    }
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
        qCCritical(FADE) << "Couldn't set new child: Unknown current ptr!:"
                         << static_cast<void *>(currentChild);
    }
}

bool FadeLayer::started() {
    return m_currentTimeDifferenceMs > 0;
}

bool FadeLayer::finished() {
    return m_currentTimeDifferenceMs > m_durationMs;
}
