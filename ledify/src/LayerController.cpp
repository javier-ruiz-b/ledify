#include "LayerController.h"
#include "ColorLayer.h"
#include "FadeLayer.h"
#include "RandomLayer.h"
#include <QtDebug>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(LAYER_CONTROL, "ledify.layercontroller", QtDebugMsg)

LayerController::LayerController() {
    reset();
}

void LayerController::reset() {
    m_indexedLayers.clear();
    m_rootLayer.reset();
}

void LayerController::setAsRootLayer(uint16_t index) {
    if (!m_indexedLayers.contains(index)) {
        qCCritical(LAYER_CONTROL) << "Doesn't exist " << index;
        return;
    }

    m_rootLayer.setChild(m_indexedLayers.take(index));
}

void LayerController::addFadeLayer(QSharedPointer<Layer> from, QSharedPointer<Layer> to, uint16_t myIndex, uint16_t startDelayMs, FadeLayer::Interpolator interpolator, uint16_t durationMs) {
    if (from.isNull() || to.isNull()) {
        qCCritical(LAYER_CONTROL) << static_cast<void *>(from.data())
                         << "or " << static_cast<void *>(to.data()) << "are null";
        return;
    }
    QSharedPointer<FadeLayer> layer(new FadeLayer());
    layer->setParams(from,
                     to,
                     interpolator,
                     startDelayMs,
                     durationMs);
    m_indexedLayers[myIndex] = layer;
}

void LayerController::addFadeLayer(uint16_t myIndex, uint16_t fromIndex, uint16_t toIndex, uint16_t startDelayMs, FadeLayer::Interpolator interpolator, uint16_t durationMs) {
    addFadeLayer(m_indexedLayers.take(fromIndex),
                 m_indexedLayers.take(toIndex),
                 myIndex, startDelayMs, interpolator, durationMs);
}

uint16_t LayerController::addFadeLayer(uint16_t indexFrom, uint16_t indexTo, uint16_t startDelayMs, FadeLayer::Interpolator interpolator, uint16_t durationMs) {
    uint16_t index = getFreeIndex();
    addFadeLayer(index, indexFrom, indexTo, startDelayMs, interpolator, durationMs);
    return index;
}

void LayerController::addFadeLayerFromCurrent(uint16_t myIndex, uint16_t toIndex, uint16_t startDelayMs, FadeLayer::Interpolator interpolator, uint16_t durationMs) {
    addFadeLayer(m_rootLayer.child(),
                 m_indexedLayers.take(toIndex),
                 myIndex, startDelayMs, interpolator, durationMs);
}

uint16_t LayerController::addFadeLayerFromCurrent(uint16_t toIndex, uint16_t startDelayMs, FadeLayer::Interpolator interpolator, uint16_t durationMs) {
    uint16_t index = getFreeIndex();
    addFadeLayerFromCurrent(index, toIndex, startDelayMs, interpolator, durationMs);
    return index;
}

void LayerController::addColorLayer(uint16_t toIndex, uint16_t r, uint16_t g, uint16_t b, uint16_t w) {
    QSharedPointer<ColorLayer> layer(new ColorLayer());
    layer->setColor(static_cast<unsigned char>(r),
                    static_cast<unsigned char>(g),
                    static_cast<unsigned char>(b),
                    static_cast<unsigned char>(w));
    m_indexedLayers[toIndex] = layer;
}

uint16_t LayerController::addColorLayer(uint16_t r, uint16_t g, uint16_t b, uint16_t w) {
    uint16_t index = getFreeIndex();
    addColorLayer(index, r, g, b, w);
    return index;
}

void LayerController::addRandomLayer(int toIndex) {
    QSharedPointer<RandomLayer> layer(new RandomLayer());
    m_indexedLayers[toIndex] = layer;
}

uint16_t LayerController::addRandomLayer() {
    uint16_t index = getFreeIndex();
    addRandomLayer(index);
    return index;
}

StartLayer *LayerController::rootLayer() {
    return &m_rootLayer;
}

uint16_t LayerController::getFreeIndex() {
    uint16_t index;
    do {
        index = static_cast<uint16_t>(qrand() % 65536);
    } while (m_indexedLayers.contains(index));
    return index;
}
