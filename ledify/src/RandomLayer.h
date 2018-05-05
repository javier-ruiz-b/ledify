#pragma once
#include "Layer.h"

class RandomLayer : public Layer {
public:
    RandomLayer();

    virtual uint32_t pixel(uint16_t);

    virtual void startDraw() {}
    virtual void endDraw() {}
    virtual void setNewChild(Layer *currentChild, QSharedPointer<Layer> newChild);

};
