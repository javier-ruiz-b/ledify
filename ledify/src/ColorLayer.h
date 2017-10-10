#pragma once
#include "Layer.h"
#include "definitions.h"

class ColorLayer : public Layer {
public:
    ColorLayer(byte r, byte g, byte b, byte w);

    virtual void startDraw() {}
    virtual void endDraw() {}

    virtual uint32 pixel(uint16) { return m_pixel; }
    void setColor(byte r, byte g, byte b, byte w);

private:
    uint32 m_pixel;
};
