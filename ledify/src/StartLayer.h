#pragma once
#include "Layer.h"

class StartLayer : Layer {
public:
    StartLayer();

    Layer *child() const;
    void setChild(Layer *child);

    virtual uint32 pixel(uint16 index);
    virtual void startDraw();
    virtual void endDraw();
    virtual void setNewChild(Layer *currentChild, Layer *newChild);
    virtual void setInUse(bool value);

private:
    Layer *m_child;
};
