#include "FakeNeoPixelLib.h"

#ifndef ARDUINO
Adafruit_NeoPixel::Adafruit_NeoPixel(uint16 n, uint8 p, uint16 neoPixelType) {
    pixels = nullptr;
    updateLength(n);
    pin = p;
}

Adafruit_NeoPixel::Adafruit_NeoPixel() {
    pixels = nullptr;
}

Adafruit_NeoPixel::~Adafruit_NeoPixel()
{
    if (pixels) {
        delete[] pixels;
    }
}

void Adafruit_NeoPixel::clear() {
    for (int i = 0; i < numLEDs; i++) {
        pixels[i] = 0;
    }
}

void Adafruit_NeoPixel::updateLength(uint16 n) {
    if (pixels) {
        delete[] pixels;
    }
    pixels = new uint8[n];
    numLEDs = n;
}

uint8 Adafruit_NeoPixel::getPin() { return pin; }

uint16 Adafruit_NeoPixel::numPixels() const {
    return numLEDs;
}

uint32 Adafruit_NeoPixel::getPixelColor(uint16 n) const {
    if (n >= numLEDs) {
        return 0;
    }
    return pixels[n];
}
#endif
