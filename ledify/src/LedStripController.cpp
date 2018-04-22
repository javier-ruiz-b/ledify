#include "LedStripController.h"

#include "Layer.h"
#include <cstdio>

LedStripController::LedStripController() {
}

CommandReader &LedStripController::commandReader() {
    return m_commandReader;
}

bool LedStripController::writeChar(char c) {
    if (m_commandReader.receivedChar(c)) {
        return parseCommand();
    }
    return false;
}

void LedStripController::draw(uint32_t *ledsRgbw, int numLeds) {
    m_fpsCalculator.tick();
    StartLayer *rootLayer = m_layerController.rootLayer();

    rootLayer->startDraw();
    Layer *child = rootLayer->child();
    if (child == nullptr) {
        return;
    }
    for (uint16_t i = 0; i < numLeds; i++) {
        *ledsRgbw = child->pixel(i);
        ledsRgbw++;
    }
    rootLayer->endDraw();
}

bool LedStripController::parseCommand() {
    const char *command = m_commandReader.command();
    unsigned char lengthCommand = 0;
    if ((lengthCommand = startsWith(command, "SET")) != 0) {
        commandSet(command, lengthCommand);
    } else if ((lengthCommand = startsWith(command, "COLOR")) != 0) {
        commandColor(lengthCommand, command);
    } else if ((lengthCommand = startsWith(command, "FADE")) != 0) {
        commandFade(command, lengthCommand);
    } else if ((lengthCommand = startsWith(command, "FPS")) != 0) {
        commandFps(lengthCommand, command);
    } else if ((lengthCommand = startsWith(command, "TIME")) != 0) {
        qDebug() << m_time.millis();
    } else if ((lengthCommand = startsWith(command, "RESET")) != 0) {
        m_layerController.reset();
    } else {
        qCritical() << "Unknown command:" << command;
        return false;
    }
    return true;
}

void LedStripController::commandSet(const char *command, unsigned char lengthCommand) {
    uint16_t index;
    const char *value = &command[lengthCommand + 1];
    sscanf(value, "%hu", &index);

    m_layerController.setAsRootLayer(index);
}

void LedStripController::commandColor(unsigned char lengthCommand, const char *command) {
    uint16_t r, g, b, w, index;
    const char *value = &command[lengthCommand + 1];
    sscanf(value, "%hu,%hu,%hu,%hu,%hu", &index, &r, &g, &b, &w);
    if (index >= AVAILABLE_LAYERS_NUM) {
        qCritical() << "Index range:" << command;
        return;
    }
    m_layerController.addColorLayer(index, r, g, b, w);
}

void LedStripController::commandFade(const char *command, unsigned char lengthCommand) {
    uint16_t myIndex, indexFrom, indexTo, interpolatorUint, startDelayMs, durationMs;
    const char *value = &command[lengthCommand + 1];
    sscanf(value, "%hu,%hu,%hu,%hu,%hu,%hu", &myIndex, &indexFrom, &indexTo, &interpolatorUint, &startDelayMs,  &durationMs);
    if ((indexFrom >= AVAILABLE_LAYERS_NUM) || (indexTo >= AVAILABLE_LAYERS_NUM)) {
        qCritical() << "Index range:" << command;
        return;
    }
    FadeLayer::Interpolator interpolator = static_cast<FadeLayer::Interpolator>(interpolatorUint);
    m_layerController.addFadeLayer(myIndex, indexFrom, indexTo, startDelayMs, interpolator, durationMs);
}

void LedStripController::commandFps(unsigned char lengthCommand, const char *command) {
    uint16_t enabledInt;
    const char *value = &command[lengthCommand + 1];
    sscanf(value, "%hu", &enabledInt);
    m_fpsCalculator.setEnabled(enabledInt != 0);
}

unsigned char LedStripController::startsWith(const char *string, const char *startsWithString) {
    unsigned char i;
    for (i = 0; startsWithString[i] != '\0'; i++) {
        if (string[i] != startsWithString[i]) {
            return 0;
        }
    }

    if (i != 0) {
        qDebug() << "Recv:" << string;
    }
    return i;
}

LayerController &LedStripController::layerController() {
    return m_layerController;
}
