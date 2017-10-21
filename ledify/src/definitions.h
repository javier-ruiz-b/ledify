#pragma once
#include <stdio.h>
#include <string.h>
#define DEBUG


typedef unsigned char byte;
typedef unsigned char uint8;
typedef unsigned long long int uint64;
typedef signed long long int int64;
typedef unsigned long int uint32;
typedef signed long int int32;
typedef unsigned short uint16;
typedef signed short int16;
#ifndef __PIC32MX__
#ifndef __linux
    /* chipKIT-specific code goes here */
typedef unsigned int size_t;
#endif
#endif

#ifndef nullptr
#define nullptr 0
#endif

#define SIZE_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
#define SIZE_OF_STRING(x) (SIZE_OF(x) - 1)

#ifdef ARDUINO
#include "Arduino.h"
#endif

#ifndef ARDUINO
#define MOCKABLE_TIME
void setMockedTime(bool mockedTime);
uint32 getRealTimeMicroseconds();
uint32 micros();
uint32 millis();
void setMockMicros(uint32 timeUs);
void setMockMillis(uint32 timeMs);
#endif

extern char logbuffer[64];
#ifdef DEBUG
#ifdef ARDUINO
#define logdebug(format, ...) sprintf (logbuffer, format, __VA_ARGS__); Serial.println(logbuffer)
#define logerr(format, ...) sprintf (logbuffer, format, __VA_ARGS__); Serial.println(logbuffer)
#else
#define logdebug(format, ...) printf("    "); printf (format, __VA_ARGS__); printf("\n")
#define logerr(format, ...) printf("  "); fprintf (stderr, format, __VA_ARGS__); printf("\n")
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

