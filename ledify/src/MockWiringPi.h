#pragma once
#include "IWiringPi.h"

class MockWiringPi : public IWiringPi {
public:
    void digitalWrite (int, int) {}
    void pinMode (int, int) {}
};
