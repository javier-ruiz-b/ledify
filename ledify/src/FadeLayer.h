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
    FadeLayer(Layer *source, Layer *destination, Interpolator interpolator, uint32 startTimeMs, uint16 durationMs);

    virtual void startDraw();
    virtual void endDraw();
    bool finished();

    uint32 pixel(uint16);

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
