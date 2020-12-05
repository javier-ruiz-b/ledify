#pragma once
#include "Layer.h"
#include <QVector>
#include <stdint.h>

class AdditionLayer : public Layer {
public:
    AdditionLayer(QVector<QSharedPointer<Layer>> layers);
    virtual ~AdditionLayer() override;

    virtual void draw(QVector<quint32> &buffer) override;

private:
    QVector<QSharedPointer<Layer>> m_layers;
    QVector<QVector<quint32>> m_buffers;

};
