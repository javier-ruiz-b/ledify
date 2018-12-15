#pragma once
#include "Layer.h"

class RandomLayer : public Layer {
public:
    RandomLayer();

    virtual void draw(uint32_t *buffer, uint32_t size) override;
    virtual bool animationFinished() override { return false; }

};
