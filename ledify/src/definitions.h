#pragma once
//#include <stdio.h>
//#include <stdint.h>
//#include <string.h>
//#define DEBUG

//#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
//#define SIZE_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
//#define SIZE_OF_STRING(x) (SIZE_OF(x) - 1)

//#ifdef ARDUINO
//#define nullptr 0
//#include "Arduino.h"
//#endif

//#define MOCKABLE_TIME
//namespace tempus {
//void setMockedTime(bool mockedTime);
//unsigned long getRealTimeMicroseconds();
//unsigned long micros();
//unsigned long millis();
//void setMockMicros(unsigned long timeUs);
//void setMockMillis(unsigned long timeMs);
//}

//extern char logbuffer[64];
//#ifdef DEBUG
//#define logdebug(format, ...) printf("    "); printf (format, __VA_ARGS__); printf("\n")
//#define logerr(format, ...) printf("  "); fprintf (stderr, format, __VA_ARGS__); fprintf (stderr, "\n")
//#else
//#ifdef ARDUINO
//#define logerr(format, ...) sprintf (logbuffer, format, __VA_ARGS__); Serial.println(logbuffer)
//#else
//#define logerr(format, ...) printf("  "); fprintf (stderr, format, __VA_ARGS__); printf("\n")
//#endif
//#define logdebug(format, ...)
////#define logerr(format, ...)
//#endif

//#ifdef ARDUINO
//#define print(format, ...) sprintf (logbuffer, format, __VA_ARGS__); Serial.print(logbuffer)
//#else
//#define print(format, ...) fprintf (stderr, format, __VA_ARGS__)
//#endif

