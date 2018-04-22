#pragma once
#include "Layer.h"

class TimeControl;

class FadeLayer : public Layer {

public:
    enum Interpolator {
        InterpolatorLinear = 0,
        InterpolatorAccelerate = 1,
        InterpolatorDecelerate = 2
    };

public:
    FadeLayer();

    void setParams(Layer *source, Layer *destination, Interpolator interpolator, uint16_t startTimeMs, uint16_t durationMs);
    bool finished();

    uint32_t pixel(uint16_t index);
    virtual void startDraw();
    virtual void endDraw();
    virtual void setNewChild(Layer *currentChild, Layer *newChild);
    virtual void setInUse(bool value);

private:
    inline unsigned char interpolatedDestinationValue();
    inline unsigned char interpolatedAcceleratedValue();
    inline unsigned char interpolatedDeceleratedValue();

private:
    TimeControl *m_time;
    Layer *m_source;
    Layer *m_destination;
    unsigned long m_startMs;
    uint16_t m_currentTimeDifferenceMs;
    uint16_t m_durationMs;
    Interpolator m_interpolator;

    void recalculateTimeDifference();

    friend class LedStripControllerTest;
};
