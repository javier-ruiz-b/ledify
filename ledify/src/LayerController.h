#pragma once

#include "Layer.h"
#include "FadeLayer.h"
#include "StartLayer.h"
#include <QHash>
#include <QSharedPointer>

class LayerController {

public:
    LayerController();

    void setAsRootLayer(uint16_t index);
    void addFadeLayer(uint16_t toIndex, uint16_t indexFrom, uint16_t indexTo, Interpolator::Type interpolator, uint16_t startDelayMs, uint16_t durationMs);
    uint16_t addFadeLayer(uint16_t indexFrom, uint16_t indexTo, Interpolator::Type interpolator, uint16_t startDelayMs, uint16_t durationMs);
    void addFadeLayer(QSharedPointer<Layer> from, QSharedPointer<Layer> to, uint16_t myIndex, Interpolator::Type interpolator, uint16_t startDelayMs, uint16_t durationMs);
    void addFadeLayerFromCurrent(uint16_t toIndex, uint16_t indexTo, Interpolator::Type interpolator, uint16_t startDelayMs, uint16_t durationMs);
    uint16_t addFadeLayerFromCurrent(uint16_t indexTo, Interpolator::Type interpolator, uint16_t startDelayMs, uint16_t durationMs);
    void addColorLayer(uint16_t toIndex, uint16_t r, uint16_t g, uint16_t b, uint16_t w);
    uint16_t addColorLayer(uint16_t r, uint16_t g, uint16_t b, uint16_t w);
    void addRandomLayer(int toIndex);
    uint16_t addLayer(QSharedPointer<Layer> layer);
    void copyLayer(int toIndex, int fromIndex);
    void moveLayer(int toIndex, int fromIndex);
    uint16_t addRandomLayer();
    StartLayer *rootLayer();
    uint16_t getFreeIndex();
    void reset();

private:
    StartLayer m_rootLayer;
    QHash<int, QSharedPointer<Layer>> m_indexedLayers;

    friend class LedStripControllerTest;
};
