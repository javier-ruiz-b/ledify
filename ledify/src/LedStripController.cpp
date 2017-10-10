#include "LedStripController.h"

#ifdef ARDUINO
#include <Adafruit_NeoPixel.h>
#else
#include "FakeNeoPixelLib.h"
#endif
#include "Layer.h"
#include "ColorLayer.h"
#include "FadeLayer.h"

#include <stdio.h>

LedStripController::LedStripController(Adafruit_NeoPixel *neoPixelLib)
    : m_neoPixelLib(neoPixelLib) {
    m_rootLayer = nullptr;
    for (byte i = 0; i < AVAILABLE_LAYERS_NUM; i++) {
        m_availableLayers[i] = nullptr;
    }
}

LedStripController::~LedStripController() {
    for (byte i = 0; i < AVAILABLE_LAYERS_NUM; i++) {
        delete m_availableLayers[i];
    }
    delete m_rootLayer;
}

CommandReader &LedStripController::commandReader() {
    return m_commandReader;
}

void LedStripController::writeChar(char c) {
    if (m_commandReader.writeChar(c)) {
        parseCommand();
    }
}

void LedStripController::draw() {
    if (m_rootLayer == nullptr) {
        return;
    }
    m_rootLayer->startDraw();
    for (uint16 i = 0; i < m_neoPixelLib->numPixels(); i++) {
        m_neoPixelLib->setPixelColor(i, m_rootLayer->pixel(i));
    }
    m_rootLayer->endDraw();
}

void LedStripController::commandColor(byte lengthCommand, const char *command) {
    uint16 r, g, b, w, index;
    const char *value = &command[lengthCommand + 1];
    sscanf(value, "%hu,%hu,%hu,%hu,%hu", &index, &r, &g, &b, &w);
    m_availableLayers[index] = new ColorLayer((uint8)r, (uint8)g, (uint8)b, (uint8)w);
}

void LedStripController::commandSet(const char *command, byte lengthCommand) {
    uint16 index;
    const char *value = &command[lengthCommand + 1];
    sscanf(value, "%hu", &index);
    if (m_rootLayer) {
        delete m_rootLayer;
    }
    m_rootLayer = m_availableLayers[index];
    m_availableLayers[index] = nullptr;
}

void LedStripController::parseCommand() {
    const char *command = m_commandReader.command();
    byte lengthCommand;
    if ((lengthCommand = startsWith(command, "COLOR")) != 0) {
        commandColor(lengthCommand, command);
    } else if ((lengthCommand = startsWith(command, "SET")) != 0) {
        commandSet(command, lengthCommand);
    } else if ((lengthCommand = startsWith(command, "FADE")) != 0) {
        uint16 myIndex, indexFrom, indexTo, interpolator, startDelayMs, durationMs;
        const char *value = &command[lengthCommand + 1];
        sscanf(value, "%hu,%hu,%hu,%hu,%hu,%hu", &myIndex, &indexFrom, &indexTo, &interpolator, &startDelayMs,  &durationMs);
        Layer *from = m_availableLayers[indexFrom];
        Layer *to = m_availableLayers[indexTo];
        uint32 startTimeMs = millis() + startDelayMs;
        m_availableLayers[myIndex] = new FadeLayer(from, to, (FadeLayer::Interpolator) interpolator, startTimeMs, durationMs);
        m_availableLayers[indexFrom] = nullptr;
        m_availableLayers[indexTo] = nullptr;
    }
}

byte LedStripController::startsWith(const char *string, const char *startsWithString) {
    byte i;
    for (i = 0; startsWithString[i] != '\0'; i++) {
        if (string[i] != startsWithString[i]) {
            return 0;
        }
    }
    return i;
}
