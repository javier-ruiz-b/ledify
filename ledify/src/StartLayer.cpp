#include "StartLayer.h"
#include <QtDebug>

StartLayer::StartLayer()
    : m_child(nullptr) {}

void StartLayer::reset() {
    m_child.clear();
}

QSharedPointer<Layer> StartLayer::child() const {
    return m_child;
}

void StartLayer::setChild(QSharedPointer<Layer> child) {
    setNewChild(nullptr, child);
}

uint32_t StartLayer::pixel(uint16_t index) {
    return m_child->pixel(index);
}

void StartLayer::startDraw() {
    if (!m_child.isNull()) {
        m_child->startDraw();
    }
}

void StartLayer::endDraw() {
//    if (m_child)
        m_child->endDraw();
}

bool StartLayer::animationFinished() {
    if (!m_child.isNull()) {
        return m_child->animationFinished();
    }
    return false;
}

void StartLayer::setNewChild(Layer *, QSharedPointer<Layer> newChild) {
//    qDebug() << "StartLayer new child " << static_cast<void *>(this) << static_cast<void *>(newChild);
    m_child = newChild;
    if (!m_child.isNull()) {
        m_child->setParent(this);
    }
}
