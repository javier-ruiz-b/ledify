#include "SlideAnimationLayer.h"

SlideAnimationLayer::SlideAnimationLayer(QSharedPointer<Layer> layer, float speed)
    : m_layer(layer), m_speed(speed) {}

void SlideAnimationLayer::setNewChild(Layer *, QSharedPointer<Layer> newChild) {
    newChild->setParent(this);
    m_layer = newChild;
}
