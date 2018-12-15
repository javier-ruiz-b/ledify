#pragma once
#include "ILedStrip.h"
#include "rpi_ws281x/ws2811.h"
#include <chrono>

class Layer;

class Ws2811LedStrip : public ILedStrip {
public:
    Ws2811LedStrip(uint32_t numLeds);
    void initialize () override;
    void deinitialize () override;
    void render (Layer *buffer) override;
    bool isAnyLedOn() override;

private:
    void waitIfNecessary();

private:
    uint32_t *m_ledBuffer = nullptr;
    ws2811_t m_ledStrip;
    uint32_t m_numLeds;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_lastRendered;
};
