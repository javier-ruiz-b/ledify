#pragma once
#include "Layer.h"
class Color;

class ColorLayer : public Layer {
public:
    ColorLayer(const Color &color);
    void setColor(const Color &color);

    virtual void draw(uint32_t *buffer, uint32_t size) override;
    virtual bool animationFinished() override { return true; }
    virtual void setNewChild(Layer *currentChild, QSharedPointer<Layer> newChild) override;

private:
    uint32_t m_pixel;

    friend class LedStripControllerTest;
};
