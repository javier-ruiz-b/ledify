#pragma once
#include "definitions.h"

class Layer {
public:
    Layer() : m_inUse(false) {}

    virtual uint32 pixel(uint16 index) = 0;

    virtual void startDraw() = 0;
    virtual void endDraw() = 0;
    virtual void setNewChild(Layer *currentChild, Layer *newChild) = 0;

    void setParent(Layer *parent);

    bool isInUse() const;
    virtual void setInUse(bool value) = 0;

protected:
    bool m_inUse;

private:
    Layer *m_parent;

};
