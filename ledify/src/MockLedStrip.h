#pragma once

#include "ILedStrip.h"

class MockLedStrip : public ILedStrip {
public:
    void initialize () override {}
    void deinitialize () override {}
    void render (Layer *) override {}
    bool isAnyLedOn() override { return true; }
};
