#pragma once
#include "Layer.h"
#include <Interpolator.h>

class Color;

class SpotLayer : public Layer {
public:
    SpotLayer();
    void setParams(const Color &color, float position, float size, Interpolator::Type interpolator);

    virtual uint32_t pixel(uint16_t);

    virtual void startDraw();
    virtual void endDraw() {}
    virtual bool animationFinished() { return true; }
    virtual void setNewChild(Layer *currentChild, QSharedPointer<Layer> newChild);

 private:
    uint32_t *m_pixelBuffer = nullptr;
    uint32_t m_bufferSize;
    float m_position;

    friend class LedStripControllerTest;
};
