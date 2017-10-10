#pragma once
#ifndef ARDUINO
#include "definitions.h"

class Adafruit_NeoPixel {
public:
    Adafruit_NeoPixel(uint16 n, uint8 p=6, uint16 neoPixelType = 0);
    Adafruit_NeoPixel(void);
    ~Adafruit_NeoPixel();

    void begin(void);
    void show(void);
    void setPin(uint8 p);
    void setPixelColor(uint16 n, uint8 r, uint8 g, uint8 b, uint8 w);
    void setPixelColor(uint16 n, uint32 c);
    void setBrightness(uint8);
    void clear();
    void updateLength(uint16 n);
    void updateType(uint16 neoPixelType);
    uint8 *getPixels(void) const;
    uint8 getBrightness(void) const;
    uint8 getPin(void);
    uint16 numPixels(void) const;
    static uint32 Color(uint8 r, uint8 g, uint8 b);
    static uint32 Color(uint8 r, uint8 g, uint8 b, uint8 w);
    uint32 getPixelColor(uint16 n) const;
    inline bool canShow(void) { return true; }

private:
    uint16
      numLEDs,       // Number of RGB LEDs in strip
      numBytes;      // Size of 'pixels' buffer below (3 or 4 bytes/pixel)
    uint8
      pin;           // Output pin number (-1 if not yet set)
    uint8
      brightness;      // Output pin number (-1 if not yet set)
    uint32
     *pixels;
};

#endif
