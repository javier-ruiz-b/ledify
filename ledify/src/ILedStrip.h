#pragma once
#include <stdint.h>
class Layer;
class ILedStrip {
public:
    ILedStrip(uint32_t numLeds) : m_numLeds (numLeds) {}
    virtual ~ILedStrip() = default;
    virtual void initialize () = 0;
    virtual void deinitialize () = 0;
    virtual void render (Layer *rootLayer) = 0;
    virtual void draw (uint32_t *ledBuffer) = 0;
    virtual bool isAnyLedOn() = 0;

    uint32_t numLeds() const {
        return m_numLeds;
    }

protected:
    uint32_t m_numLeds;
};


