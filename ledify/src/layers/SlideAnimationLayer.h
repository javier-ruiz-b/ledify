#pragma once
#include "Layer.h"

class SlideAnimationLayer : public Layer {
public:
    SlideAnimationLayer(QSharedPointer<Layer> layer, float speed);
    virtual ~SlideAnimationLayer() override;

    virtual void draw(uint32_t *buffer, uint32_t size) override;
    virtual bool animationFinished() override { return false; }
    virtual void setNewChild(Layer *, QSharedPointer<Layer> newChild) override;

private:
    QSharedPointer<Layer> m_layer;
    uint32_t *m_tempBuffer = nullptr;
    uint32_t m_startMs;
    float m_speed;
};
