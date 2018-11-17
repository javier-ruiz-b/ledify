#pragma once
#include "ILedStrip.h"
#include "rpi_ws281x/ws2811.h"

class Layer;

class Ws2811LedStrip : public ILedStrip {
public:
    Ws2811LedStrip(int numLeds);
    void initialize ();
    void deinitialize ();
    void render (Layer *buffer);

private:
    uint32_t *m_ledBuffer;
    ws2811_t m_ledStrip;
    int m_numLeds;
};
