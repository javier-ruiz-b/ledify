#pragma once
#include "Layer.h"

class FadeLayer : public Layer {

public:
    enum Interpolator {
        InterpolatorLinear = 0,
        InterpolatorAccelerate = 1,
        InterpolatorDecelerate = 2
    };

public:
    FadeLayer() : m_startMs(0), m_interpolator(InterpolatorLinear) {}
    void setParams(Layer *source, Layer *destination, Interpolator interpolator, uint32 startTimeMs, uint16 durationMs);

    void startPixel();
    void endPixel();
    uint32 pixel(uint16);
    bool finished();

private:
    inline byte interpolatedDestinationValue();
    inline byte interpolatedAcceleratedValue();
    inline byte interpolatedDeceleratedValue();

private:
    Layer *m_source;
    Layer *m_destination;
    uint32 m_startMs;
    uint16 m_currentTimeDifferenceMs;
    uint16 m_durationMs;
    Interpolator m_interpolator;

};
