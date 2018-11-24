#pragma once
#include "Layer.h"

class RandomLayer : public Layer {
public:
    RandomLayer();

    virtual uint32_t pixel(uint16_t) override;

    virtual void startDraw() override {}
    virtual void draw(uint32_t *buffer, uint32_t size) override;
    virtual void endDraw() override {}
    virtual bool animationFinished() override { return false; }
    virtual void setNewChild(Layer *, QSharedPointer<Layer> newChild) override;

};
