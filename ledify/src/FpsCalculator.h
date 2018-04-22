#pragma once

#include <QtCore>

class TimeControl;

class FpsCalculator {
public:
    FpsCalculator();

    void tick();
    void setEnabled(bool enabled);

private:
    void check();

private:
    bool m_enabled;
    uint32_t m_startTimeMs;
    uint32_t m_ticks;
    TimeControl *m_time;

};
