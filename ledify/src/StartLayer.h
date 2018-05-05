#pragma once
#include "Layer.h"
#include <QSharedPointer>

class StartLayer : Layer {
public:
    StartLayer();

    void reset();

    QSharedPointer<Layer> child() const;
    void setChild(QSharedPointer<Layer> child);

    virtual uint32_t pixel(uint16_t index);
    virtual void startDraw();
    virtual void endDraw();
    virtual void setNewChild(Layer *currentChild, QSharedPointer<Layer> newChild);

private:
    QSharedPointer<Layer> m_child;
};
