#include "LayerController.h"

template <typename T>
T *LayerController::findFreeLayer(T *layers, LayerType type, byte numLayers) {
    for (byte i = 0; i < numLayers; i++) {
        if (!layers[i].isInUse()) {
            layers[i].setInUse(true);
            return &layers[i];
        }
    }
    logerr("No available layer for %d", (int) type);
    return nullptr;
}

LayerController::LayerController() {
    reset();
}

void LayerController::reset() {
    reset(m_color, NUM_COLOR);
    reset(m_fade, NUM_FADE);
    for (byte i = 0; i < AVAILABLE_LAYERS_NUM; i++) {
        m_availableLayers[i] = nullptr;
    }
}

void LayerController::setAsRootLayer(uint16 index) {
    if (index >= AVAILABLE_LAYERS_NUM) {
        logerr("Index range: %d", index);
        return;
    }
    if (m_availableLayers[index] == nullptr) {
        logerr("Doesn't exist %i", index);
        return;
    }

    m_rootLayer.setChild(m_availableLayers[index]);
    m_availableLayers[index] = nullptr;
}

void LayerController::addFadeLayer(uint16 myIndex, uint16 fromIndex, uint16 toIndex, uint16 startDelayMs, FadeLayer::Interpolator interpolator, uint16 durationMs) {
    Layer *from = m_availableLayers[fromIndex];
    Layer *to = m_availableLayers[toIndex];
    if ((from == nullptr) || (to == nullptr)) {
        logerr("%p or %p are null", static_cast<void *>(from), static_cast<void *>(to));
        return;
    }
    FadeLayer *layer;
    if ((layer = static_cast<FadeLayer *>(setAvailableLayer(LayerController::Fade, myIndex)))) {
        layer->setParams(from,
                         to,
                         interpolator,
                         startDelayMs,
                         durationMs);
    }
    m_availableLayers[fromIndex] = nullptr;
    m_availableLayers[toIndex] = nullptr;
}

void LayerController::addColorLayer(uint16 toIndex, uint16 b, uint16 r, uint16 w, uint16 g) {
    ColorLayer *layer;
    if ((layer = static_cast<ColorLayer *>(setAvailableLayer(LayerController::Color, toIndex)))) {
        layer->setColor(static_cast<uint8>(r),
                        static_cast<uint8>(g),
                        static_cast<uint8>(b),
                        static_cast<uint8>(w));
    }
}

StartLayer *LayerController::rootLayer() {
    return &m_rootLayer;
}

Layer *LayerController::getLayer(LayerController::LayerType type) {
    switch (type) {
    case Color:
        return findFreeLayer(m_color, type, NUM_COLOR);
    case Fade:
        return findFreeLayer(m_fade, type, NUM_FADE);
    }
    return nullptr;
}

Layer *LayerController::setAvailableLayer(LayerController::LayerType layerType, uint16 index) {
    Layer *layer = getLayer(layerType);
    if (layer == nullptr) {
        return nullptr;
    }
    if (m_availableLayers[index]) {
        m_availableLayers[index]->setInUse(false);
    }
    m_availableLayers[index] = layer;
    return layer;
}

template<typename T>
void LayerController::reset(T *layers, int numLayers) {
    for (byte i = 0; i < numLayers; i++) {
        layers[i].setInUse(false);
    }
}
