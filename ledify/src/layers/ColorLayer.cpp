#include "ColorLayer.h"
#include <QtDebug>
#include <QLoggingCategory>
#include <Color.h>

Q_LOGGING_CATEGORY(COLOR, "ledify.color", QtWarningMsg)

ColorLayer::ColorLayer(const Color &color) {
    m_pixel = color.rgbw();
}

void ColorLayer::setColor(const Color &color) {
    m_pixel = color.rgbw();
}

void ColorLayer::draw(uint32_t *buffer, uint32_t size) {
    for(uint32_t i = 0; i < size; i++)  {
        buffer[i] = m_pixel;
    }
}

void ColorLayer::setNewChild(Layer *, QSharedPointer<Layer> newChild) {
    qCCritical(COLOR) << "Unexpected: ColorLayer receiving child %p" << newChild;
}
