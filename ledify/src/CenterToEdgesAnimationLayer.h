#pragma once
#include "Layer.h"
#include "Interpolator.h"

class TimeControl;

class CenterToEdgesAnimationLayer : public Layer {
public:
    CenterToEdgesAnimationLayer();

    void setParams(QSharedPointer<Layer> background, Interpolator::Type interpolator, uint32_t color, uint16_t pixels, uint16_t durationMs, float endDistance);

    uint32_t pixel(uint16_t index) override;
    virtual void startDraw() override;
    virtual void endDraw() override;
    virtual bool animationFinished() override { return false; }
    virtual void setNewChild(Layer *currentChild, QSharedPointer<Layer> newChild) override;


private:
    TimeControl *m_time;
    QSharedPointer<Layer> m_background;
    Interpolator::Type m_interpolator;
    uint32_t m_color;
    uint16_t m_pixels;
    bool m_startSet = false;
    uint16_t m_startMs;
    uint16_t m_currentTimeDifferenceMs;
    uint16_t m_durationMs;
    float m_endDistance;

    friend class LedStripControllerTest;
};
