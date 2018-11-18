#pragma once
#include <stdint.h>
#include <QSharedPointer>

class Layer {
public:
//    Layer() {}
    virtual ~Layer() = default;

    virtual uint32_t pixel(uint16_t index) = 0;

    virtual void startDraw() = 0;
    virtual void endDraw() = 0;
    virtual bool animationFinished() = 0;

    virtual void setNewChild(Layer *currentChild, QSharedPointer<Layer> newChild) = 0;
    void setParent(Layer *parent);

protected:
    Layer *m_parent;

};