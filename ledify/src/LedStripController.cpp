#include "LedStripController.h"

#include "Layer.h"
#include <cstdio>
#include <QTimer>
#include <QString>

Q_LOGGING_CATEGORY(CONTROLLER, "ledify.controller", QtWarningMsg)

LedStripController::LedStripController() {
    m_layerController.addColorLayer(0, 0, 0, 0, 0);
    m_layerController.setAsRootLayer(0);
    m_executor.reset(new CommandExecutor(&m_layerController, &m_fpsCalculator));
}

CommandReader &LedStripController::commandReader() {
    return m_commandReader;
}

bool LedStripController::writeChar(char c) {
    if (m_commandReader.receivedChar(c)) {
        return parseReceivedString(m_commandReader.command()).isEmpty();
    }
    return false;
}

bool LedStripController::animationFinished() {
    return m_layerController.rootLayer()->animationFinished();
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
        ledsRgbw[i] = child->pixel(i);
    }
    rootLayer->endDraw();
}

QString LedStripController::parseReceivedString(const QString &string) {
    qCDebug(CONTROLLER) << "Recv:" << string;
    auto commandAndArgs = string.split("=");
    if (commandAndArgs.empty()) {
        qCDebug(CONTROLLER) << "Empty command";
        return "Empty command";
    }
    auto command = commandAndArgs.first();
    QStringList args;
    if (commandAndArgs.count() >= 2) {
        args = commandAndArgs[1].split(",");
    }

    QString response;
    m_executor->parseCommand(command, args, response);
    return response;
}

void LedStripController::commandOff() {
    QString str;
    m_executor->cOff(QStringList(), str);
}

void LedStripController::commandOnIfNight() {
    QString str;
    m_executor->cOnIfNight(QStringList(), str);
}

LayerController &LedStripController::layerController() {
    return m_layerController;
}
