#include "Layer.h"

Layer::Layer()
    : m_parent(nullptr), m_finished(false) {}

void Layer::finish(bool propagate) {
    m_finished = true;
    if (propagate && m_parent) {
        m_parent->finish(true);
    }
}

void Layer::setParent(Layer *parent) {
    m_parent = parent;
}
