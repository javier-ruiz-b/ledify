#pragma once
#include "Layer.h"

class ColorLayer : public Layer {
public:
    ColorLayer();
    void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char w);

    virtual uint32_t pixel(uint16_t);

    virtual void startDraw() {}
    virtual void endDraw() {}
    virtual void setNewChild(Layer *currentChild, QSharedPointer<Layer> newChild);

private:
    uint32_t m_pixel;

    friend class LedStripControllerTest;
};
