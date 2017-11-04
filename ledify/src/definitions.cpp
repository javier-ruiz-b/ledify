#include "definitions.h"

//#ifdef DEBUG
char logbuffer[64];
//#endif

#ifndef ARDUINO
#include <sys/time.h>

bool m_mockTime = false;
unsigned long m_mockedTimeUs = 0;

void setMockedTime(bool mockedTime) {
    m_mockTime = mockedTime;
}

unsigned long getRealTimeMicroseconds() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (unsigned long) (1000000 * tv.tv_sec + tv.tv_usec);
}

unsigned long micros() {
    if (m_mockTime) {
        return m_mockedTimeUs;
    } else {
        return getRealTimeMicroseconds();
    }
}

unsigned long millis() {
    if (m_mockTime) {
        return m_mockedTimeUs/1000;
    } else {
        return getRealTimeMicroseconds()/1000;
    }
}

void setMockMicros(unsigned long timeUs) {
    m_mockedTimeUs = timeUs;
}

void setMockMillis(unsigned long timeMs) {
    m_mockedTimeUs = timeMs * 1000;
}
#endif
