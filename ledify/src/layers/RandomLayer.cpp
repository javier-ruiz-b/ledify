#include "RandomLayer.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(RANDOM, "ledify.random", QtWarningMsg)

RandomLayer::RandomLayer() {}

void RandomLayer::draw(QVector<quint32> &buffer) {
    for (int i = 0; i < buffer.size(); i++) {
        buffer[i] = static_cast<uint32_t>(qrand() | (qrand() & 0xFF) << 16);
    }
}
