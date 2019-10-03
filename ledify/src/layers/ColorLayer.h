#pragma once
#include "Layer.h"
class Color;

class ColorLayer : public Layer {
public:
    ColorLayer(const Color &color);
    static ColorLayer *createFromCommand(const QStringList &args, QString &);

    void setColor(const Color &color);

    virtual void draw(uint32_t *buffer, uint32_t size) override;

private:
    uint32_t m_pixel;

    friend class LedStripControllerTest;
};
