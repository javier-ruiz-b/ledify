#pragma once
#include "Layer.h"
class Color;

class ColorLayer : public Layer {
public:
    ColorLayer();
    void setColor(const Color &color);

    virtual uint32_t pixel(uint16_t);

    virtual void startDraw() {}
    virtual void endDraw() {}
    virtual bool animationFinished() { return true; }
    virtual void setNewChild(Layer *currentChild, QSharedPointer<Layer> newChild);

private:
    uint32_t m_pixel;

    friend class LedStripControllerTest;
};
