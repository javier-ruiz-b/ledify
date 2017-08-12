#pragma once
#include "Layer.h"
#include "definitions.h"

class ColorLayer : public Layer {
public:
    ColorLayer() {}

    virtual uint32 pixel(uint16) { return m_pixel; }
    void setColor(byte w, byte r, byte g, byte b);

private:
    uint32 m_pixel;
};
