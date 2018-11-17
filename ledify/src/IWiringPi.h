#pragma once

class IWiringPi {

public:
    virtual ~IWiringPi() = default;
    virtual void digitalWrite (int pin, int value) = 0;
    virtual void pinMode (int pin, int mode) = 0;

    enum PinMode {
        input,
        output,
        low,
        high
    };
};
