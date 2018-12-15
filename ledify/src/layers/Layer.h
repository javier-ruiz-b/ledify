#pragma once
#include <stdint.h>
#include <QSharedPointer>

class Layer {
public:
    virtual ~Layer() = default;

    virtual void draw(uint32_t *buffer, uint32_t size) = 0;

    virtual bool animationFinished();
    virtual void setNewChild(Layer *currentChild, QSharedPointer<Layer> newChild);

    void setParent(Layer *parent);

protected:
    Layer *m_parent = nullptr;

private:
    uint32_t pixel(uint32_t i);

    friend class ColorLayerTest;
    friend class FadeLayerTest;
    friend class LedStripControllerTest;
};
