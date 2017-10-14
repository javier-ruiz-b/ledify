#pragma once
#include "Layer.h"

class StartLayer : Layer {
public:
    StartLayer() : m_child(nullptr) {}

    void setChild(Layer *child);
    Layer *child() const;

    virtual uint32 pixel(uint16 index);
    virtual void startDraw();
    virtual void endDraw();
    virtual void setNewChild(Layer *currentChild, Layer *newChild);
    virtual void setInUse(bool value);

private:
    Layer *m_child;
};
