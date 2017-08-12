#pragma once
#include "definitions.h"

class Layer {
public:
    Layer() {}

    virtual uint32 pixel(uint16) = 0;

protected:


};
