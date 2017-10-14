#include "StartLayer.h"

void StartLayer::setChild(Layer *child) {
    m_child = child;
}

Layer *StartLayer::child() const {
    return m_child;
}

uint32 StartLayer::pixel(uint16 index) {
    return m_child->pixel(index);
}

void StartLayer::startDraw() {
    m_child->startDraw();
}

void StartLayer::endDraw() {
    m_child->endDraw();
}

void StartLayer::setNewChild(Layer *, Layer *newChild) {
    m_child = newChild;
}

void StartLayer::setInUse(bool value) {
    m_inUse = value;
    if (m_child) {
        m_child->setInUse(value);
    }
    if (!value) {
        m_child = nullptr;
    }
}
