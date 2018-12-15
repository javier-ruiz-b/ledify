#include "AdditionLayer.h"
#include <Color.h>

AdditionLayer::AdditionLayer(QVector<QSharedPointer<Layer>> layers)
    : m_layers(layers) {
}

AdditionLayer::~AdditionLayer() {
    for (auto buffer : m_buffers) {
        delete[] buffer;
    }
}

void AdditionLayer::draw(uint32_t *buffer, uint32_t size) {
    if (m_buffers.empty()) {
        m_buffers.reserve(m_layers.size());
        for (int i = 0; i < m_layers.size(); i++) {
            m_buffers.append(new uint32_t[size]);
        }
    }

    for (int i = 0; i < m_layers.size(); i++) {
        m_layers.value(i)->draw(m_buffers[i], size);
    }

    for (unsigned int i = 0; i < size; i++) {
        Color colorSum(0);
        for (int j = 0; j < m_layers.size(); j++) {
            Color color(m_buffers[j][i]);
            colorSum = colorSum.add(color);
        }
        buffer[i] = colorSum.rgbw();
    }
}
