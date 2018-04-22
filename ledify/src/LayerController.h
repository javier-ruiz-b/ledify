#pragma once

#include "Layer.h"
#include "ColorLayer.h"
#include "FadeLayer.h"
#include "StartLayer.h"

#define NUM_COLOR 32
#define NUM_FADE 16
#define AVAILABLE_LAYERS_NUM   64

class LayerController {

public:
    enum LayerType { Color, Fade };

public:
    LayerController();

    void reset();

    void setAsRootLayer(uint16_t index);
    void addFadeLayer(uint16_t toIndex, uint16_t indexFrom, uint16_t indexTo, uint16_t startDelayMs, FadeLayer::Interpolator interpolator, uint16_t durationMs);
    uint16_t addFadeLayer(uint16_t indexFrom, uint16_t indexTo, uint16_t startDelayMs, FadeLayer::Interpolator interpolator, uint16_t durationMs);
    void addFadeLayerFromCurrent(uint16_t toIndex, uint16_t indexTo, uint16_t startDelayMs, FadeLayer::Interpolator interpolator, uint16_t durationMs);
    uint16_t addFadeLayerFromCurrent(uint16_t indexTo, uint16_t startDelayMs, FadeLayer::Interpolator interpolator, uint16_t durationMs);
    void addColorLayer(uint16_t toIndex, uint16_t r, uint16_t g, uint16_t b, uint16_t w);
    uint16_t addColorLayer(uint16_t r, uint16_t g, uint16_t b, uint16_t w);
    StartLayer *rootLayer();
    uint16_t getFreeIndex();

private:
    Layer *getLayer(LayerType type);
    Layer *setAvailableLayer(LayerController::LayerType layerType, uint16_t index);

    template <typename T>
    inline T *findFreeLayer(T *layers, LayerType type, unsigned char numLayers);

    template <typename T>
    inline void reset(T *layers, int numLayers);

private:
    StartLayer m_rootLayer;
    Layer *m_availableLayers[AVAILABLE_LAYERS_NUM];
    ColorLayer m_color[NUM_COLOR];
    FadeLayer m_fade[NUM_FADE];
    int m_lastFreeIndexHint = 0;

    friend class LedStripControllerTest;
    void addFadeLayer(Layer *from, Layer *to, uint16_t myIndex, uint16_t startDelayMs, FadeLayer::Interpolator interpolator, uint16_t durationMs);
};
