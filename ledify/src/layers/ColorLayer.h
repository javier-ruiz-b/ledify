#pragma once
#include "Layer.h"
class Color;

class ColorLayer : public Layer {
public:
    ColorLayer(const Color &color);
    static ColorLayer *createFromCommand(const QStringList &args, QString &);

    void setColor(const Color &color);

    virtual void draw(QVector<quint32> &buffer) override;

private:
    uint32_t m_pixel;

    friend class LedStripControllerTest;
};
