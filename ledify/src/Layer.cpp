#include "Layer.h"

void Layer::setParent(Layer *parent) {
    m_parent = parent;
}

bool Layer::isInUse() const {
    return m_inUse;
}
