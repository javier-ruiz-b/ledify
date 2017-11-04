#pragma once

#include "Layer.h"
#include "ColorLayer.h"
#include "FadeLayer.h"

#define NUM_COLOR 8
#define NUM_FADE 4

class LayerController {

public:
    enum LayerType { Color, Fade };

public:
    LayerController() {}

    Layer *getLayer(LayerType type);
    void reset();

private:
    template <typename T>
    inline T *findFreeLayer(T *layers, LayerType type, byte numLayers);

    template <typename T>
    inline void reset(T *layers, int numLayers);

private:
    ColorLayer m_color[NUM_COLOR];
    FadeLayer m_fade[NUM_FADE];
};
