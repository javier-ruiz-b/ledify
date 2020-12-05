#pragma once
#include "ILedStrip.h"
#include "rpi_ws281x/ws2811.h"
#include <QVector>

class Layer;

class Ws2811LedStrip : public ILedStrip {
public:
    Ws2811LedStrip();
    void initialize () override;
    void deinitialize () override;
    void render (Layer *buffer) override;
    bool isAnyLedOn() override;

private:
    QVector<quint32> m_ledBuffer;
    uint32_t m_ledCount;
    ws2811_t m_ledStrip;
};
