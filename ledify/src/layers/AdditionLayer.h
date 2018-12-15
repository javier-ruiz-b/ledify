#pragma once
#include "Layer.h"
#include <QVector>
#include <stdint.h>

class AdditionLayer : public Layer {
public:
    AdditionLayer(QVector<QSharedPointer<Layer>> layers);
    virtual ~AdditionLayer() override;

    virtual void draw(uint32_t *buffer, uint32_t size) override;

private:
    QVector<QSharedPointer<Layer>> m_layers;
    QVector<uint32_t *> m_buffers;

};
