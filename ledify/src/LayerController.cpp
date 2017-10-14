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

Layer *LayerController::getLayer(LayerController::LayerType type) {
    switch (type) {
    case Color:
        return findFreeLayer(m_color, type, NUM_COLOR);
    case Fade:
        return findFreeLayer(m_fade, type, NUM_FADE);
    }
}
