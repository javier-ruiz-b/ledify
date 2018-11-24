#include "ColorLayer.h"
#include <QtDebug>
#include <QLoggingCategory>
#include <Color.h>

Q_LOGGING_CATEGORY(COLOR, "ledify.color", QtWarningMsg)

ColorLayer::ColorLayer(const Color &color) {
    m_pixel = color.pixel();
}

void ColorLayer::setColor(const Color &color) {
    m_pixel = color.pixel();
}

uint32_t ColorLayer::pixel(uint16_t) {
    return m_pixel;
}

void ColorLayer::setNewChild(Layer *, QSharedPointer<Layer> newChild) {
    qCCritical(COLOR) << "Unexpected: ColorLayer receiving child %p" << newChild;
}
