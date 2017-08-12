#pragma once

typedef unsigned char byte;
typedef unsigned long long int uint64;
typedef signed long long int int64;
typedef unsigned long int uint32;
typedef signed long int int32;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned int size_t;

#ifndef nullptr
#define nullptr 0
#endif

#define SIZE_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
#define SIZE_OF_STRING(x) (SIZE_OF(x) - 1)

#ifndef ARDUINO
#define MOCKABLE_TIME
void setMockedTime(bool mockedTime);
uint32 getRealTimeMicroseconds();
uint32 micros();
uint32 millis();
void setMockMicros(uint32 timeUs);
void setMockMillis(uint32 timeMs);
#endif

