#pragma once
#include "definitions.h"

class Layer {
public:
    Layer();

    virtual uint32 pixel() = 0;
    void finish(bool propagate);
    void setParent(Layer *parent);

protected:
    bool m_finished;

private:
    Layer *m_parent;

};
