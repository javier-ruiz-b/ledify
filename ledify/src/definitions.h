#pragma once
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#define DEBUG

typedef uint8_t byte;
typedef uint8_t uint8;
typedef uint64_t uint64;
typedef int64_t int64;
typedef uint32_t uint32;
typedef int32_t int32;
typedef uint16_t uint16;
typedef int16_t int16;
#ifndef __PIC32MX__
#ifndef __linux
    /* chipKIT-specific code goes here */
typedef unsigned int size_t;
#endif
#endif

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define SIZE_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
#define SIZE_OF_STRING(x) (SIZE_OF(x) - 1)

#ifdef ARDUINO
#define nullptr 0
#include "Arduino.h"
#endif

#define MOCKABLE_TIME
namespace tempus {
void setMockedTime(bool mockedTime);
unsigned long getRealTimeMicroseconds();
unsigned long micros();
unsigned long millis();
void setMockMicros(unsigned long timeUs);
void setMockMillis(unsigned long timeMs);
}

extern char logbuffer[64];
#ifdef DEBUG
#ifdef ARDUINO
#define logdebug(format, ...) sprintf (logbuffer, format, __VA_ARGS__); Serial.println(logbuffer)
#define logerr(format, ...) sprintf (logbuffer, format, __VA_ARGS__); Serial.println(logbuffer)
#else
#define logdebug(format, ...) printf("    "); printf (format, __VA_ARGS__); printf("\n")
#define logerr(format, ...) printf("  "); fprintf (stderr, format, __VA_ARGS__); fprintf (stderr, "\n")
#endif
#else
#ifdef ARDUINO
#define logerr(format, ...) sprintf (logbuffer, format, __VA_ARGS__); Serial.println(logbuffer)
#else
#define logerr(format, ...) printf("  "); fprintf (stderr, format, __VA_ARGS__); printf("\n")
#endif
#define logdebug(format, ...)
//#define logerr(format, ...)
#endif

#ifdef ARDUINO
#define print(format, ...) sprintf (logbuffer, format, __VA_ARGS__); Serial.print(logbuffer)
#else
#define print(format, ...) fprintf (stderr, format, __VA_ARGS__)
#endif

