#include "ColorLayer.h"
#include <QtDebug>
#include <QLoggingCategory>
#include <Color.h>

Q_LOGGING_CATEGORY(COLOR, "ledify.color", QtWarningMsg)

ColorLayer::ColorLayer(const Color &color) {
    m_pixel = color.rgbw();
}

ColorLayer *ColorLayer::createFromCommand(const QStringList &args, QString &) {
    return new ColorLayer(Color(args[1].toUShort(),
                                args[2].toUShort(),
                                args[3].toUShort(),
                                args[4].toUShort()));
}

void ColorLayer::setColor(const Color &color) {
    m_pixel = color.rgbw();
}

void ColorLayer::draw(uint32_t *buffer, uint32_t size) {
    for(uint32_t i = 0; i < size; i++)  {
        buffer[i] = m_pixel;
    }
}
