#pragma once

#include "Layer.h"
#include "ColorLayer.h"
#include "FadeLayer.h"
#include "StartLayer.h"
#include <QHash>
#include <QSet>
#include <QSharedPointer>

#define NUM_COLOR 32
#define NUM_FADE 16

class LayerController {

public:
    LayerController();

    void setAsRootLayer(uint16_t index);
    void addFadeLayer(uint16_t toIndex, uint16_t indexFrom, uint16_t indexTo, uint16_t startDelayMs, FadeLayer::Interpolator interpolator, uint16_t durationMs);
    uint16_t addFadeLayer(uint16_t indexFrom, uint16_t indexTo, uint16_t startDelayMs, FadeLayer::Interpolator interpolator, uint16_t durationMs);
    void addFadeLayer(QSharedPointer<Layer> from, QSharedPointer<Layer> to, uint16_t myIndex, uint16_t startDelayMs, FadeLayer::Interpolator interpolator, uint16_t durationMs);
    void addFadeLayerFromCurrent(uint16_t toIndex, uint16_t indexTo, uint16_t startDelayMs, FadeLayer::Interpolator interpolator, uint16_t durationMs);
    uint16_t addFadeLayerFromCurrent(uint16_t indexTo, uint16_t startDelayMs, FadeLayer::Interpolator interpolator, uint16_t durationMs);
    void addColorLayer(uint16_t toIndex, uint16_t r, uint16_t g, uint16_t b, uint16_t w);
    uint16_t addColorLayer(uint16_t r, uint16_t g, uint16_t b, uint16_t w);
    StartLayer *rootLayer();
    uint16_t getFreeIndex();
    void reset();

private:
    StartLayer m_rootLayer;
    QHash<int, QSharedPointer<Layer>> m_indexedLayers;

    friend class LedStripControllerTest;
};
