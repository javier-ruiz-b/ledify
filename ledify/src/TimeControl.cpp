#include "TimeControl.h"

TimeControl TimeControl::s_instance;

TimeControl *TimeControl::instance() {
    return &s_instance;
}

TimeControl::TimeControl() {
    m_programStart = chrono::system_clock::now();
}

void TimeControl::setMillis(uint32_t millis) {
    m_millis = millis;
}

void TimeControl::setMocked(bool mocked) {
    m_mocked = mocked;
}

uint32_t TimeControl::millis() const {
    if (m_mocked) {
        return m_millis;
    } else {
        auto now = chrono::system_clock::now();
        auto durationMs = now - m_programStart;
        return static_cast<uint32_t>(durationMs.count());
    }
}
