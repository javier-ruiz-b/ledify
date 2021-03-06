#pragma once
#include "Layer.h"
#include <QSharedPointer>

class StartLayer : public Layer {
public:
    StartLayer(QSharedPointer<Layer> child = nullptr);

    void reset();

    QSharedPointer<Layer> child() const;
    void setChild(QSharedPointer<Layer> child);

    virtual void draw(QVector<quint32> &buffer) override;
    virtual bool animationFinished() override;
    virtual void setNewChild(Layer *currentChild, QSharedPointer<Layer> newChild) override;

private:
    QSharedPointer<Layer> m_child;
};
