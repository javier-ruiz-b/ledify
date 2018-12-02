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

void LayerController::setAsRoot(Layer *layer) {
    m_rootLayer.setChild(QSharedPointer<Layer>(layer));
}

void LayerController::addTo(uint16_t toIndex, Layer *layer) {
    m_indexedLayers[toIndex] = QSharedPointer<Layer>(layer);
}

uint16_t LayerController::add(Layer *layer) {
    uint16_t toIndex = getFreeIndex();
    m_indexedLayers[toIndex] = QSharedPointer<Layer>(layer);
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
    return m_indexedLayers.value(index);
}

QSharedPointer<Layer> LayerController::take(int index) {
    return m_indexedLayers.take(index);
}

StartLayer &LayerController::root() {
    return m_rootLayer;
}

QSharedPointer<Layer> LayerController::current() {
    return m_rootLayer.child();
}

uint16_t LayerController::getFreeIndex() {
    for (uint16_t i = 1; i < UINT16_MAX; i++) {
        if (m_indexedLayers.contains(i)) {
            return i;
        }
    }
    return 0;
}
