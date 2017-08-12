#pragma once
#include "Layer.h"
const byte LINEAR_INTERPOLATOR = 0;

class FadeLayer : public Layer {
public:
    FadeLayer() : m_startMs(0), m_interpolator(LINEAR_INTERPOLATOR) {}
    void setSourceLayer(Layer *source) { m_source = source; }
    void setDestinationLayer(Layer *destination) { m_destination = destination; }
    void setStartTimeMs(uint32 timeMs) { m_startMs = timeMs; }
    void setDuration(uint16 durationMs) { m_durationMs = durationMs; }
    void setInterpolator(byte interpolatorType);


    uint32 pixel(uint16);

private:
    inline byte interpolatedDestinationValue(uint32 timeMs);

private:
    Layer *m_source;
    Layer *m_destination;
    uint32 m_startMs;
    uint32 m_durationMs;
    byte m_interpolator;

};
