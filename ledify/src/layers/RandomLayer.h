#pragma once
#include "Layer.h"

class RandomLayer : public Layer {
public:
    RandomLayer();

    virtual void draw(QVector<quint32> &buffer) override;
    virtual bool animationFinished() override { return false; }

};
