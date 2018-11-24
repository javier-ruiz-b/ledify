#pragma once
#include "Layer.h"
#include "Interpolator.h"

class TimeControl;

class FadeLayer : public Layer {

public:
    FadeLayer(QSharedPointer<Layer> source, QSharedPointer<Layer> destination, Interpolator::Type interpolator, uint16_t startTimeMs, uint16_t durationMs);

    void setParams(QSharedPointer<Layer> source, QSharedPointer<Layer> destination, Interpolator::Type interpolator, uint16_t startTimeMs, uint16_t durationMs);
    bool finished();

    uint32_t pixel(uint16_t index) override;
    virtual void startDraw() override;
    virtual void endDraw() override;
    virtual bool animationFinished() override { return false; }
    virtual void setNewChild(Layer *currentChild, QSharedPointer<Layer> newChild) override;

private:
    inline unsigned char interpolatedDestinationValue();
    void recalculateTimeDifference();

private:
    TimeControl *m_time;
    QSharedPointer<Layer> m_source;
    QSharedPointer<Layer> m_destination;
    bool m_startSet = false;
    uint32_t m_startMs;
    uint16_t m_delayFromStart;
    uint16_t m_currentTimeDifferenceMs;
    uint16_t m_durationMs;
    Interpolator::Type m_interpolator;
    unsigned char m_alphaDestination;

    friend class LedStripControllerTest;
};
