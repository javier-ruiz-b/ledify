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
    void addFadeLayer(uint16_t toIndex, uint16_t indexFrom, uint16_t indexTo, Interpolator::Type interpolator, uint16_t startDelayMs, uint16_t durationMs);
    uint16_t addFadeLayer(uint16_t indexFrom, uint16_t indexTo, Interpolator::Type interpolator, uint16_t startDelayMs, uint16_t durationMs);
    void addFadeLayer(QSharedPointer<Layer> from, QSharedPointer<Layer> to, uint16_t myIndex, Interpolator::Type interpolator, uint16_t startDelayMs, uint16_t durationMs);
    void addFadeLayerFromCurrent(uint16_t toIndex, uint16_t indexTo, Interpolator::Type interpolator, uint16_t startDelayMs, uint16_t durationMs);
    uint16_t addFadeLayerFromCurrent(uint16_t indexTo, Interpolator::Type interpolator, uint16_t startDelayMs, uint16_t durationMs);
    void addColorLayer(uint16_t toIndex, uint16_t r, uint16_t g, uint16_t b, uint16_t w);
    uint16_t addColorLayer(uint16_t r, uint16_t g, uint16_t b, uint16_t w);
    void addTo(uint16_t toIndex, Layer *layer);
    uint16_t add(Layer *layer);
    uint16_t add(QSharedPointer<Layer> layer);
    void copy(int toIndex, int fromIndex);
    void move(int toIndex, int fromIndex);
    QSharedPointer<Layer> at(int index);
    StartLayer &root();

private:
    uint16_t getFreeIndex();
    StartLayer m_rootLayer;
    QHash<int, QSharedPointer<Layer>> m_indexedLayers;

    friend class LedStripControllerTest;
};
