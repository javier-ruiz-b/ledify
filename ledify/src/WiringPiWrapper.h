#pragma once
#include "IWiringPi.h"

class WiringPiWrapper : public IWiringPi {
public:
    WiringPiWrapper();

    void digitalWrite (int pin, int value) override;
    void pinMode (int pin, int mode) override;
};
