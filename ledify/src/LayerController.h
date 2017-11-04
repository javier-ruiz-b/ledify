#pragma once

#include "Layer.h"
#include "ColorLayer.h"
#include "FadeLayer.h"
#include "StartLayer.h"

#define NUM_COLOR 8
#define NUM_FADE 4
#define AVAILABLE_LAYERS_NUM   8

class LayerController {

public:
    enum LayerType { Color, Fade };

public:
    LayerController();

    void reset();

    void setAsRootLayer(uint16 index);
    void addFadeLayer(uint16 toIndex, uint16 indexFrom, uint16 indexTo, uint16 startDelayMs, FadeLayer::Interpolator interpolator, uint16 durationMs);
    void addColorLayer(uint16 toIndex, uint16 b, uint16 r, uint16 w, uint16 g);
    StartLayer *rootLayer();

private:
    Layer *getLayer(LayerType type);
    Layer *setAvailableLayer(LayerController::LayerType layerType, uint16 index);

    template <typename T>
    inline T *findFreeLayer(T *layers, LayerType type, byte numLayers);

    template <typename T>
    inline void reset(T *layers, int numLayers);

private:
    StartLayer m_rootLayer;
    Layer *m_availableLayers[AVAILABLE_LAYERS_NUM];
    ColorLayer m_color[NUM_COLOR];
    FadeLayer m_fade[NUM_FADE];

    friend class LedStripControllerTest;
};
