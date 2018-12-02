#include "SlideAnimationLayer.h"

#include <QtDebug>
#include <TimeControl.h>

SlideAnimationLayer::SlideAnimationLayer(QSharedPointer<Layer> layer, float speed)
    : m_layer(layer), m_speed(speed) {}

SlideAnimationLayer::~SlideAnimationLayer() {
    delete[] m_tempBuffer;
}

void SlideAnimationLayer::draw(uint32_t *buffer, uint32_t size) {
    m_layer->draw(buffer, size);
    if (!m_tempBuffer) {
        m_tempBuffer = new uint32_t[size];
    }
    if (!m_startSet) {
        m_startSet = true;
        m_startMs = TimeControl::instance()->millis();
        return;
    }
    auto now = TimeControl::instance()->millis();
    auto diffAndSpeed = static_cast<float>(now - m_startMs) * m_speed;
    uint32_t offset = static_cast<uint32_t>(diffAndSpeed) % size;
//    qDebug() << now << offset;
    memcpy(&m_tempBuffer[offset], buffer, (size - offset) * sizeof(uint32_t));
    if (offset) {
        memcpy(m_tempBuffer, &buffer[size-offset], (offset)* sizeof(uint32_t));
    }
    memcpy(buffer, m_tempBuffer, size * sizeof(uint32_t));
}

void SlideAnimationLayer::setNewChild(Layer *, QSharedPointer<Layer> newChild) {
    newChild->setParent(this);
    m_layer = newChild;
}
