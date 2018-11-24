#pragma once
#include "Layer.h"
#include <Interpolator.h>

class Color;

class SpotLayer : public Layer {
public:
    SpotLayer(const Color &color, float position, float size, Interpolator::Type interpolator);

    virtual uint32_t pixel(uint16_t) override;

    virtual void startDraw() override;
    virtual void draw(uint32_t *buffer, uint32_t size) override;
    virtual void endDraw() override {}
    virtual bool animationFinished() override { return true; }
    virtual void setNewChild(Layer *currentChild, QSharedPointer<Layer> newChild) override;

 private:
    uint32_t *m_pixelBuffer = nullptr;
    uint32_t m_bufferSize;
    float m_position;

    friend class LedStripControllerTest;
};
