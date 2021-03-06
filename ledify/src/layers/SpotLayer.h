#pragma once
#include "Layer.h"
#include <Interpolator.h>

class Color;

class SpotLayer : public Layer {
public:
    SpotLayer(const Color &color, float position, float size, Interpolator::Type interpolator);
    virtual ~SpotLayer() override;

    virtual void draw(QVector<quint32> &buffer) override;

private:
    uint32_t *m_pixelBuffer = nullptr;
    uint32_t m_bufferSize;
    float m_position;

    friend class LedStripControllerTest;
};
