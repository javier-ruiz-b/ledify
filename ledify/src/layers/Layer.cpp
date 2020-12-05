#include "Layer.h"
#include <QScopedPointer>
#include <QtTest/QtTest>

bool Layer::animationFinished() {
    return true;
}

void Layer::setNewChild(Layer *currentChild, QSharedPointer<Layer> newChild) {
    qCritical() << "Unexpected: Layer " << this
                << "receiving new child" << newChild
                << "for child" << currentChild;
}

void Layer::setParent(Layer *parent) {
    m_parent = parent;
}

uint32_t Layer::pixel(uint32_t i) {
    QVector<quint32> buffer(i + 1);
    draw(buffer);
    return buffer[i];
}
