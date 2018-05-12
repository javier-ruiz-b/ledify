#include "RandomLayer.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(RANDOM, "ledify.random", QtWarningMsg)

RandomLayer::RandomLayer() {}

uint32_t RandomLayer::pixel(uint16_t) {
    return static_cast<uint32_t>(qrand() | (qrand() & 0xFF) << 16);
}

void RandomLayer::setNewChild(Layer *, QSharedPointer<Layer> newChild) {
    qCCritical(RANDOM) << "Unexpected: ColorLayer receiving child %p" << newChild;
}

