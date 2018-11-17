#pragma once

#include "ILedStrip.h"

class MockLedStrip : public ILedStrip {
public:
    void initialize () {}
    void deinitialize () {}
    void render (Layer *) {}
};
