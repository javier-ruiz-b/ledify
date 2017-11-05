#include "definitions.h"

//#ifdef DEBUG
char logbuffer[64];
//#endif

#ifdef __arm__
#include <wiringPi.h>
#include <sys/time.h>
#elif defined ARDUINO
#else
#include <sys/time.h>
#endif

bool m_mockTime = false;
unsigned long m_mockedTimeUs = 0;

void tempus::setMockedTime(bool mockedTime) {
    m_mockTime = mockedTime;
}

unsigned long tempus::getRealTimeMicroseconds() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (unsigned long) (1000000 * tv.tv_sec + tv.tv_usec);
}

unsigned long tempus::micros() {
#if defined(ARDUINO) || defined(__arm__)
    return micros();
#endif
    if (m_mockTime) {
        return m_mockedTimeUs;
    } else {
        return getRealTimeMicroseconds();
    }
}

unsigned long tempus::millis() {
#if defined(ARDUINO) || defined(__arm__)
    return ::millis();
#endif
    if (m_mockTime) {
        return m_mockedTimeUs/1000;
    } else {
        return getRealTimeMicroseconds()/1000;
    }
}

void tempus::setMockMicros(unsigned long timeUs) {
    m_mockedTimeUs = timeUs;
}

void tempus::setMockMillis(unsigned long timeMs) {
    m_mockedTimeUs = timeMs * 1000;
}

