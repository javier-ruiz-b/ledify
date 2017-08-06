#pragma once
#include "Layer.h"

class ColorLayer : Layer {
public:
    ColorLayer(Layer *parent = nullptr);

private:
    uint32 m_pixel;
};
