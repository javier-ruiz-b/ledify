#include "LayerController.h"
#include <QtDebug>

template <typename T>
T *LayerController::findFreeLayer(T *layers, LayerType type, unsigned char numLayers) {
    for (unsigned char i = 0; i < numLayers; i++) {
        if (!layers[i].isInUse()) {
            layers[i].setInUse(true);
            return &layers[i];
        }
    }
    qCritical() << "No available layer for " << (int) type;
    return nullptr;
}

LayerController::LayerController() {
    reset();
}

void LayerController::reset() {
    reset(m_color, NUM_COLOR);
    reset(m_fade, NUM_FADE);
    for (unsigned char i = 0; i < AVAILABLE_LAYERS_NUM; i++) {
        m_availableLayers[i] = nullptr;
    }
}

void LayerController::setAsRootLayer(uint16_t index) {
    if (index >= AVAILABLE_LAYERS_NUM) {
        qCritical() << "Index range:" << index;
        return;
    }
    if (m_availableLayers[index] == nullptr) {
        qCritical() << "Doesn't exist " << index;
        return;
    }

    m_rootLayer.setChild(m_availableLayers[index]);
    m_availableLayers[index] = nullptr;
}

void LayerController::addFadeLayer(Layer *from, Layer *to, uint16_t myIndex, uint16_t startDelayMs, FadeLayer::Interpolator interpolator, uint16_t durationMs) {
    if ((from == nullptr) || (to == nullptr)) {
        qCritical() << static_cast<void *>(from)
                    << "or " << static_cast<void *>(to) << "are null";
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
}

void LayerController::addFadeLayer(uint16_t myIndex, uint16_t fromIndex, uint16_t toIndex, uint16_t startDelayMs, FadeLayer::Interpolator interpolator, uint16_t durationMs) {
    Layer *from = m_availableLayers[fromIndex];
    Layer *to = m_availableLayers[toIndex];
    addFadeLayer(from, to, myIndex, startDelayMs, interpolator, durationMs);
    m_availableLayers[fromIndex] = nullptr;
    m_availableLayers[toIndex] = nullptr;
}

uint16_t LayerController::addFadeLayer(uint16_t indexFrom, uint16_t indexTo, uint16_t startDelayMs, FadeLayer::Interpolator interpolator, uint16_t durationMs) {
    uint16_t index = getFreeIndex();
    addFadeLayer(index, indexFrom, indexTo, startDelayMs, interpolator, durationMs);
    return index;
}

void LayerController::addFadeLayerFromCurrent(uint16_t myIndex, uint16_t toIndex, uint16_t startDelayMs, FadeLayer::Interpolator interpolator, uint16_t durationMs) {
    Layer *from = m_rootLayer.child();
    Layer *to = m_availableLayers[toIndex];
    addFadeLayer(from, to, myIndex, startDelayMs, interpolator, durationMs);
    m_availableLayers[toIndex] = nullptr;
}

uint16_t LayerController::addFadeLayerFromCurrent(uint16_t toIndex, uint16_t startDelayMs, FadeLayer::Interpolator interpolator, uint16_t durationMs) {
    uint16_t index = getFreeIndex();
    addFadeLayerFromCurrent(index, toIndex, startDelayMs, interpolator, durationMs);
    return index;
}

void LayerController::addColorLayer(uint16_t toIndex, uint16_t r, uint16_t g, uint16_t b, uint16_t w) {
    ColorLayer *layer;
    if ((layer = static_cast<ColorLayer *>(setAvailableLayer(LayerController::Color, toIndex)))) {
        layer->setColor(static_cast<unsigned char>(r),
                        static_cast<unsigned char>(g),
                        static_cast<unsigned char>(b),
                        static_cast<unsigned char>(w));
    }
}

uint16_t LayerController::addColorLayer(uint16_t r, uint16_t g, uint16_t b, uint16_t w) {
    uint16_t index = getFreeIndex();
    addColorLayer(index, r, g, b, w);
    return index;
}

StartLayer *LayerController::rootLayer() {
    return &m_rootLayer;
}

uint16_t LayerController::getFreeIndex() {
    while(m_availableLayers[m_lastFreeIndexHint] != nullptr) {
        m_lastFreeIndexHint = (m_lastFreeIndexHint + 1) % AVAILABLE_LAYERS_NUM;
    }
    return static_cast<uint16_t>(m_lastFreeIndexHint);
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

Layer *LayerController::setAvailableLayer(LayerController::LayerType layerType, uint16_t index) {
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
    for (unsigned char i = 0; i < numLayers; i++) {
        layers[i].setInUse(false);
    }
}
