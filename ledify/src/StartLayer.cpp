#include "StartLayer.h"

StartLayer::StartLayer()
    : m_child(nullptr) {
    setInUse(true);
}

Layer *StartLayer::child() const {
    return m_child;
}

void StartLayer::setChild(Layer *child) {
    setInUse(false);
    setNewChild(nullptr, child);
    setInUse(true);
}

uint32 StartLayer::pixel(uint16 index) {
    return m_child->pixel(index);
}

void StartLayer::startDraw() {
    if (m_child != nullptr) {
        m_child->startDraw();
    }
}

void StartLayer::endDraw() {
//    if (m_child)
        m_child->endDraw();
}

void StartLayer::setNewChild(Layer *, Layer *newChild) {
    logdebug("StartLayer(%p) new child %p", static_cast<void *>(this), static_cast<void *>(newChild));
    m_child = newChild;
    if (m_child != nullptr) {
        m_child->setParent(this);
    }
}

void StartLayer::setInUse(bool value) {
    logdebug("StartLayer(%p) %d", static_cast<void *>(this), static_cast<int>(value));
    m_inUse = value;
    if (m_child) {
        m_child->setInUse(value);
    }
    if (!value) {
        m_child = nullptr;
    }
}
