#include "TimeControl.h"
#include "wiringPi.h"

TimeControl TimeControl::s_instance;

TimeControl *TimeControl::instance() {
    return &s_instance;
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
        return ::millis();
    }
}
