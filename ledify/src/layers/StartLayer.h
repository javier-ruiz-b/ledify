#pragma once
#include "Layer.h"
#include <QSharedPointer>

class StartLayer : public Layer {
public:
    StartLayer();

    void reset();

    QSharedPointer<Layer> child() const;
    void setChild(QSharedPointer<Layer> child);

    virtual void draw(uint32_t *buffer, uint32_t size) override;
    virtual bool animationFinished() override;
    virtual void setNewChild(Layer *currentChild, QSharedPointer<Layer> newChild) override;

private:
    QSharedPointer<Layer> m_child;
};
