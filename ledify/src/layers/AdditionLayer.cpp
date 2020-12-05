#include "AdditionLayer.h"
#include <Color.h>

AdditionLayer::AdditionLayer(QVector<QSharedPointer<Layer>> layers)
    : m_layers(layers) {
//    m_buffers.reserve()
}

AdditionLayer::~AdditionLayer() {
}

void AdditionLayer::draw(QVector<quint32> &buffer) {
    if (m_buffers.empty()) {
        m_buffers.resize(m_layers.size());
        for (int i = 0; i < m_layers.size(); i++) {
            m_buffers[i].resize(buffer.size());
        }
    }

    for (int i = 0; i < m_layers.size(); i++) {
        m_layers.value(i)->draw(m_buffers[i]);
    }

    for (int i = 0; i < buffer.size(); i++) {
        buffer[i] = 0;
        for (int j = 0; j < m_layers.size(); j++) {
            auto pixel = m_buffers[j][i];
            buffer[i] = Color::add(buffer[i], pixel);
        }
    }
}
