#pragma once
#include <stdint.h>

class TimeControl {
public:
    static TimeControl *instance();

    TimeControl(bool mocked = false) : m_mocked(mocked) {}

    void setMillis(uint32_t millis);
    void setMocked(bool mocked);

    uint32_t millis() const;

private:
    static TimeControl s_instance;

    bool m_mocked = false;
    uint32_t m_millis = 0;

};
