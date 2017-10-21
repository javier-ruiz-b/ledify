#include "definitions.h"

//#ifdef DEBUG
char logbuffer[64];
//#endif

#ifndef ARDUINO
#include <sys/time.h>

bool m_mockTime = false;
uint32 m_mockedTimeUs = 0;

void setMockedTime(bool mockedTime) {
    m_mockTime = mockedTime;
}

uint32 getRealTimeMicroseconds() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (uint32) (1000000 * tv.tv_sec + tv.tv_usec);
}

uint32 micros() {
    if (m_mockTime) {
        return m_mockedTimeUs;
    } else {
        return getRealTimeMicroseconds();
    }
}

uint32 millis() {
    if (m_mockTime) {
        return m_mockedTimeUs/1000;
    } else {
        return getRealTimeMicroseconds()/1000;
    }
}

void setMockMicros(uint32 timeUs) {
    m_mockedTimeUs = timeUs;
}

void setMockMillis(uint32 timeMs) {
    m_mockedTimeUs = timeMs * 1000;
}
#endif
