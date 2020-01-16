#pragma once
#include "ILedStrip.h"
#include "rpi_ws281x/ws2811.h"

class Layer;

class Ws2811LedStrip : public ILedStrip {
public:
    Ws2811LedStrip(uint32_t numLeds);
    void initialize () override;
    void deinitialize () override;
    void render (Layer *buffer) override;
    void draw (uint32_t *ledBuffer)  override;
    bool isAnyLedOn() override;

private:
    uint32_t *m_ledBuffer = nullptr;
    ws2811_t m_ledStrip;
};
