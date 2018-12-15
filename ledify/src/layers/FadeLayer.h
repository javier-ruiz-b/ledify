#pragma once
#include "Layer.h"
#include "Interpolator.h"

class TimeControl;

class FadeLayer : public Layer {

public:
    FadeLayer(QSharedPointer<Layer> source, QSharedPointer<Layer> destination, Interpolator::Type interpolator, uint16_t startTimeMs, uint16_t durationMs);
    virtual ~FadeLayer() override;

    void setParams(QSharedPointer<Layer> source, QSharedPointer<Layer> destination, Interpolator::Type interpolator, uint16_t startTimeMs, uint16_t durationMs);
    bool started();
    bool finished();

    virtual void draw(uint32_t *buffer, uint32_t size) override;
    virtual bool animationFinished() override { return false; }
    virtual void setNewChild(Layer *currentChild, QSharedPointer<Layer> newChild) override;

private:
    inline unsigned char interpolatedDestinationValue();
    inline uint32_t drawPixel(uint32_t sourcePixel, uint32_t destinationPixel, uint16_t alphaSource, uint8_t alphaDestination);
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
    uint32_t *m_tempBuffer = nullptr;

    friend class LedStripControllerTest;
};
