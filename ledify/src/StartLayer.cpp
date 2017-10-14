#include "StartLayer.h"

Layer *StartLayer::child() const {
    return m_child;
}

uint32 StartLayer::pixel(uint16 index) {
//    if (m_child)
        return m_child->pixel(index);
//    else
//        return 0;
}

void StartLayer::startDraw() {
//    if (m_child)
        m_child->startDraw();
}

void StartLayer::endDraw() {
//    if (m_child)
        m_child->endDraw();
}

void StartLayer::setNewChild(Layer *, Layer *newChild) {
    m_child = newChild;
    m_child->setParent(this);
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
