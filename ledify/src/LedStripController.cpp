#include "LedStripController.h"

#include "Layer.h"
#include <cstdio>
#include <QTimer>
#include <wiringPi.h>

Q_LOGGING_CATEGORY(CONTROLLER, "ledify.controller", QtWarningMsg)

LedStripController::LedStripController() {
    m_layerController.addColorLayer(0, 0, 0, 0, 0);
    m_layerController.setAsRootLayer(0);
    pinMode (c_relayGpioPin, OUTPUT);
    digitalWrite (c_relayGpioPin, HIGH);
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

bool LedStripController::writeString(const QString &command) {
    for(int i = 0; i < command.length(); i++) {
        writeChar(command.at(i).toLatin1());
    }
    return writeChar('\n');
}

void LedStripController::draw(uint32_t *ledsRgbw, int numLeds) {
    m_fpsCalculator.tick();
    auto *rootLayer = m_layerController.rootLayer();

    rootLayer->startDraw();
    auto child = rootLayer->child();
    if (child.isNull()) {
        return;
    }
    for (uint16_t i = 0; i < numLeds; i++) {
        *ledsRgbw = child->pixel(i);
        ledsRgbw++;
    }
    rootLayer->endDraw();
}

void LedStripController::commandOff() {
    auto colorIndex = layerController().addColorLayer(0, 0, 0 ,0);
    auto fadeIndex = layerController().addFadeLayerFromCurrent(colorIndex, 0, FadeLayer::InterpolatorDecelerate, 1000);
    layerController().setAsRootLayer(fadeIndex);
    QTimer::singleShot(2000, nullptr, [this] { digitalWrite (c_relayGpioPin, LOW); });
}

void LedStripController::commandOn() {
    digitalWrite (c_relayGpioPin, HIGH);
    auto colorIndex = layerController().addColorLayer(60, 40, 5, 100);
    auto fadeIndex = layerController().addFadeLayerFromCurrent(colorIndex, 2000, FadeLayer::InterpolatorDecelerate, 1000);
    layerController().setAsRootLayer(fadeIndex);
}

bool LedStripController::parseCommand() {
    const char *command = m_commandReader.command();
    unsigned char lengthCommand = 0;
    if ((lengthCommand = startsWith(command, "SET")) != 0) {
        commandSet(command, lengthCommand);
    } else if ((lengthCommand = startsWith(command, "RANDOM")) != 0) {
        commandRandom(lengthCommand, command);
    } else if ((lengthCommand = startsWith(command, "COLOR")) != 0) {
        commandColor(lengthCommand, command);
    } else if ((lengthCommand = startsWith(command, "FADETO")) != 0) {
        commandFadeTo(command, lengthCommand);
    } else if ((lengthCommand = startsWith(command, "FADE")) != 0) {
        commandFade(command, lengthCommand);
    } else if ((lengthCommand = startsWith(command, "FPS")) != 0) {
        commandFps(lengthCommand, command);
    } else if ((lengthCommand = startsWith(command, "TIME")) != 0) {
        qCDebug(CONTROLLER) << m_time.millis();
    } else if ((lengthCommand = startsWith(command, "RESET")) != 0) {
        m_layerController.reset();
    } else if ((lengthCommand = startsWith(command, "OFF")) != 0) {
        commandOff();
    } else if ((lengthCommand = startsWith(command, "ON")) != 0) {
        commandOn();
    } else {
        qCCritical(CONTROLLER) << "Unknown command:" << command;
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

void LedStripController::commandRandom(unsigned char, const char *) {
    auto index = m_layerController.addRandomLayer();
    index = m_layerController.addFadeLayerFromCurrent(index, 0, FadeLayer::InterpolatorDecelerate, 1000);
    auto colorIndex = layerController().addColorLayer(60, 40, 5, 100);
    index = m_layerController.addFadeLayerFromCurrent(colorIndex, 8000, FadeLayer::InterpolatorDecelerate, 1000);
    m_layerController.setAsRootLayer(index);
}

void LedStripController::commandColor(unsigned char lengthCommand, const char *command) {
    uint16_t r, g, b, w, index;
    const char *value = &command[lengthCommand + 1];
    sscanf(value, "%hu,%hu,%hu,%hu,%hu", &index, &r, &g, &b, &w);
    m_layerController.addColorLayer(index, r, g, b, w);
}

void LedStripController::commandFade(const char *command, unsigned char lengthCommand) {
    uint16_t myIndex, indexFrom, indexTo, interpolatorUint, startDelayMs, durationMs;
    const char *value = &command[lengthCommand + 1];
    sscanf(value, "%hu,%hu,%hu,%hu,%hu,%hu", &myIndex, &indexFrom, &indexTo, &interpolatorUint, &startDelayMs,  &durationMs);
    FadeLayer::Interpolator interpolator = static_cast<FadeLayer::Interpolator>(interpolatorUint);
    m_layerController.addFadeLayer(myIndex, indexFrom, indexTo, startDelayMs, interpolator, durationMs);
}

void LedStripController::commandFadeTo(const char *command, unsigned char lengthCommand) {
    uint16_t myIndex, indexTo, interpolatorUint, startDelayMs, durationMs;
    const char *value = &command[lengthCommand + 1];
    sscanf(value, "%hu,%hu,%hu,%hu,%hu", &myIndex, &indexTo, &interpolatorUint, &startDelayMs,  &durationMs);
    FadeLayer::Interpolator interpolator = static_cast<FadeLayer::Interpolator>(interpolatorUint);
    m_layerController.addFadeLayerFromCurrent(myIndex, indexTo, startDelayMs, interpolator, durationMs);
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
        qCDebug(CONTROLLER) << "Recv:" << string;
    }
    return i;
}

LayerController &LedStripController::layerController() {
    return m_layerController;
}
