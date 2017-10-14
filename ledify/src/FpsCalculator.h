#pragma once

#include "definitions.h"

class FpsCalculator {
public:
    FpsCalculator();

    void tick();
    void setEnabled(bool enabled);

private:
    void check();

private:
    bool m_enabled;
    uint32 m_startTimeMs;
    uint32 m_ticks;
};
