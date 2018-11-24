#pragma once

#include "Layer.h"
#include "FadeLayer.h"
#include "StartLayer.h"
#include <QHash>
#include <QSharedPointer>

class LayerController {

public:
    LayerController();
    void reset();

    void setAsRoot(uint16_t index);
    void setAsRoot(Layer *layer);
    void addTo(uint16_t toIndex, Layer *layer);
    uint16_t add(Layer *layer);
    void copy(int toIndex, int fromIndex);
    void move(int toIndex, int fromIndex);
    QSharedPointer<Layer> at(int index);
    StartLayer &root();
    QSharedPointer<Layer> current();

private:
    uint16_t getFreeIndex();
    StartLayer m_rootLayer;
    QHash<int, QSharedPointer<Layer>> m_indexedLayers;

    friend class LedStripControllerTest;
};
