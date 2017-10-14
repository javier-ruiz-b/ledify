#pragma once
#include "Layer.h"
#include "definitions.h"

class ColorLayer : public Layer {
public:
    ColorLayer();
    void setColor(byte r, byte g, byte b, byte w);

    virtual uint32 pixel(uint16);

    virtual void startDraw() {}
    virtual void endDraw() {}
    virtual void setNewChild(Layer *currentChild, Layer *newChild);
    virtual void setInUse(bool value);

private:
    uint32 m_pixel;

    friend class LedStripControllerTest;
};
