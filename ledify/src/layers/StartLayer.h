#pragma once
#include "Layer.h"
#include <QSharedPointer>

class StartLayer : Layer {
public:
    StartLayer();

    void reset();

    QSharedPointer<Layer> child() const;
    void setChild(QSharedPointer<Layer> child);

    virtual uint32_t pixel(uint16_t index) override;
    virtual void startDraw() override;
    virtual void endDraw() override;
    virtual bool animationFinished() override;
    virtual void setNewChild(Layer *currentChild, QSharedPointer<Layer> newChild) override;

private:
    QSharedPointer<Layer> m_child;
};
