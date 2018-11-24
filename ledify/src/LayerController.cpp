#include "LayerController.h"
#include "ColorLayer.h"
#include "FadeLayer.h"
#include "RandomLayer.h"
#include <Color.h>
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

void LayerController::setAsRoot(uint16_t index) {
    if (!m_indexedLayers.contains(index)) {
        qCCritical(LAYER_CONTROL) << "Doesn't exist " << index;
        return;
    }

    m_rootLayer.setChild(m_indexedLayers.take(index));
}

void LayerController::addFadeLayer(QSharedPointer<Layer> from,
                                   QSharedPointer<Layer> to,
                                   uint16_t myIndex,
                                   Interpolator::Type interpolator,
                                   uint16_t startDelayMs,
                                   uint16_t durationMs) {
    if (from.isNull() || to.isNull()) {
        qCCritical(LAYER_CONTROL) << static_cast<void *>(from.data())
                         << "or " << static_cast<void *>(to.data()) << "are null";
        return;
    }
    QSharedPointer<FadeLayer> layer(new FadeLayer(from,
                                                  to,
                                                  interpolator,
                                                  startDelayMs,
                                                  durationMs));
    m_indexedLayers[myIndex] = layer;
}

void LayerController::addFadeLayer(uint16_t myIndex, uint16_t fromIndex, uint16_t toIndex, Interpolator::Type interpolator, uint16_t startDelayMs, uint16_t durationMs) {
    addFadeLayer(m_indexedLayers.take(fromIndex),
                 m_indexedLayers.take(toIndex),
                 myIndex, interpolator, startDelayMs, durationMs);
}

uint16_t LayerController::addFadeLayer(uint16_t indexFrom, uint16_t indexTo, Interpolator::Type interpolator, uint16_t startDelayMs, uint16_t durationMs) {
    uint16_t index = getFreeIndex();
    addFadeLayer(index, indexFrom, indexTo, interpolator, startDelayMs, durationMs);
    return index;
}

void LayerController::addFadeLayerFromCurrent(uint16_t myIndex, uint16_t toIndex, Interpolator::Type interpolator, uint16_t startDelayMs, uint16_t durationMs) {
    addFadeLayer(m_rootLayer.child(),
                 m_indexedLayers.take(toIndex),
                 myIndex, interpolator, startDelayMs, durationMs);
}

uint16_t LayerController::addFadeLayerFromCurrent(uint16_t toIndex, Interpolator::Type interpolator, uint16_t startDelayMs, uint16_t durationMs) {
    uint16_t index = getFreeIndex();
    addFadeLayerFromCurrent(index, toIndex, interpolator, startDelayMs, durationMs);
    return index;
}

void LayerController::addColorLayer(uint16_t toIndex, uint16_t r, uint16_t g, uint16_t b, uint16_t w) {
    QSharedPointer<ColorLayer> layer(new ColorLayer(Color(r, g, b, w)));
    m_indexedLayers[toIndex] = layer;
}

uint16_t LayerController::addColorLayer(uint16_t r, uint16_t g, uint16_t b, uint16_t w) {
    uint16_t index = getFreeIndex();
    addColorLayer(index, r, g, b, w);
    return index;
}

void LayerController::addTo(uint16_t toIndex, Layer *layer) {
    m_indexedLayers[toIndex] = QSharedPointer<Layer>(layer);
}

uint16_t LayerController::add(Layer *layer) {
    return add(QSharedPointer<Layer>(layer));
}

uint16_t LayerController::add(QSharedPointer<Layer> layer) {
    uint16_t toIndex = getFreeIndex();
    m_indexedLayers[toIndex] = layer;
    return toIndex;
}

void LayerController::copy(int toIndex, int fromIndex) {
    m_indexedLayers[toIndex] = m_indexedLayers[fromIndex];
}

void LayerController::move(int toIndex, int fromIndex) {
    m_indexedLayers[toIndex] = m_indexedLayers[fromIndex];
    m_indexedLayers[fromIndex].reset();
}

QSharedPointer<Layer> LayerController::at(int index) {
    return m_indexedLayers[index];
}

StartLayer &LayerController::root() {
    return m_rootLayer;
}

uint16_t LayerController::getFreeIndex() {
    for (uint16_t i = 1; i < UINT16_MAX; i++) {
        if (m_indexedLayers.contains(i)) {
            return i;
        }
    }
    return 0;
}
