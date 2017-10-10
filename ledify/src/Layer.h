#pragma once
#include "definitions.h"

class Layer {
public:
    Layer() {}

    virtual uint32 pixel(uint16) = 0;

    virtual void startDraw() = 0;
    virtual void endDraw() = 0;

};
