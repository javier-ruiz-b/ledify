#pragma once
#include <stdint.h>
#include <chrono>

using namespace std;

class TimeControl {
public:
    static TimeControl *instance();

    TimeControl();

    void setMillis(uint32_t millis);
    void setMocked(bool mocked);

    uint32_t millis() const;

private:
    static TimeControl s_instance;
    chrono::time_point<chrono::system_clock> m_programStart;

    bool m_mocked = false;
    uint32_t m_millis = 0;

};
