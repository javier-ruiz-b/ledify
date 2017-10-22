#include "LedStripController.h"

#include "Layer.h"
#include "ColorLayer.h"
#include "FadeLayer.h"

#include <stdio.h>

LedStripController::LedStripController() {
    for (byte i = 0; i < AVAILABLE_LAYERS_NUM; i++) {
        m_availableLayers[i] = nullptr;
    }
}

CommandReader &LedStripController::commandReader() {
    return m_commandReader;
}

bool LedStripController::writeChar(char c) {
    if (m_commandReader.writeChar(c)) {
        return parseCommand();
    }
    return false;
}

void LedStripController::draw(uint32 *ledsRgbw, int numLeds) {
    m_fpsCalculator.tick();
    if (m_rootLayer.child() == nullptr) {
        return;
    }
    m_rootLayer.startDraw();
    Layer *child = m_rootLayer.child();
    if (!child) {
        return;
    }
    for (uint16 i = 0; i < numLeds; i++) {
        *ledsRgbw = child->pixel(i);
        ledsRgbw++;
    }
    m_rootLayer.endDraw();
}

void LedStripController::commandColor(byte lengthCommand, const char *command) {
    uint16 r, g, b, w, index;
    const char *value = &command[lengthCommand + 1];
    sscanf(value, "%hu,%hu,%hu,%hu,%hu", &index, &r, &g, &b, &w);
    if (index >= AVAILABLE_LAYERS_NUM) {
        logerr("Index range: %s", command);
        return;
    }
    if (m_availableLayers[index]) {
        m_availableLayers[index]->setInUse(false);
    }
    m_availableLayers[index] = m_layerController.getLayer(LayerController::Color);
    if (m_availableLayers[index] == nullptr) {
        return;
    }
    ((ColorLayer *) m_availableLayers[index])->setColor((uint8)r, (uint8)g, (uint8)b, (uint8)w);
}

void LedStripController::commandSet(const char *command, byte lengthCommand) {
    uint16 index;
    const char *value = &command[lengthCommand + 1];
    sscanf(value, "%hu", &index);
    if (index >= AVAILABLE_LAYERS_NUM) {
        logerr("Index range: %s", command);
        return;
    }
    if (m_availableLayers[index] == nullptr) {
        logerr("Doesn't exist %i", index);
        return;
    }

    m_rootLayer.setInUse(false);
    m_rootLayer.setNewChild(nullptr, m_availableLayers[index]);
    m_availableLayers[index] = nullptr;
}

void LedStripController::commandFade(const char *command, byte lengthCommand) {
    uint16 myIndex, indexFrom, indexTo, interpolator, startDelayMs, durationMs;
    const char *value = &command[lengthCommand + 1];
    sscanf(value, "%hu,%hu,%hu,%hu,%hu,%hu", &myIndex, &indexFrom, &indexTo, &interpolator, &startDelayMs,  &durationMs);
    if ((indexFrom >= AVAILABLE_LAYERS_NUM) || (indexTo >= AVAILABLE_LAYERS_NUM)) {
        logerr("Index range: %s", command);
        return;
    }
    Layer *from = m_availableLayers[indexFrom];
    Layer *to = m_availableLayers[indexTo];
    if ((from == nullptr) || (to == nullptr)) {
        logerr("%p or %p are null", from, to);
        return;
    }
    uint32 startTimeMs = millis() + startDelayMs;
    if (m_availableLayers[myIndex]) {
        m_availableLayers[myIndex]->setInUse(false);
    }
    m_availableLayers[myIndex] = m_layerController.getLayer(LayerController::Fade);
    if (m_availableLayers[myIndex] == nullptr) {
        return;
    }
    ((FadeLayer *)m_availableLayers[myIndex])->setParams(from, to, (FadeLayer::Interpolator) interpolator, startTimeMs, durationMs);
    m_availableLayers[indexFrom] = nullptr;
    m_availableLayers[indexTo] = nullptr;
}

void LedStripController::commandFps(byte lengthCommand, const char *command) {
    uint16 enabledInt;
    const char *value = &command[lengthCommand + 1];
    sscanf(value, "%hu", &enabledInt);
    m_fpsCalculator.setEnabled(enabledInt != 0);
}

bool LedStripController::parseCommand() {
    const char *command = m_commandReader.command();
    byte lengthCommand = 0;
    if ((lengthCommand = startsWith(command, "SET")) != 0) {
        commandSet(command, lengthCommand);
    } else if ((lengthCommand = startsWith(command, "COLOR")) != 0) {
        commandColor(lengthCommand, command);
    } else if ((lengthCommand = startsWith(command, "FADE")) != 0) {
        commandFade(command, lengthCommand);
    } else if ((lengthCommand = startsWith(command, "FPS")) != 0) {
        commandFps(lengthCommand, command);
    } else if ((lengthCommand = startsWith(command, "TIME")) != 0) {
        print("%u", (unsigned int)(millis()));
    } else {
        logerr("Unknown: %s", command);
        return false;
    }
    if (lengthCommand != 0) {
        print("OK: %s", command);
    }
    return true;
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
