#pragma once
#include <stdint.h>
class Layer;
class ILedStrip {
public:
    ILedStrip() {}
    virtual ~ILedStrip() = default;
    virtual void initialize () = 0;
    virtual void deinitialize () = 0;
    virtual void render (Layer *rootLayer) = 0;
    virtual bool isAnyLedOn() = 0;

};
