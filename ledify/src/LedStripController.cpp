#include "LedStripController.h"

#include "Layer.h"
#include <cstdio>
#include <QTimer>
#include <QString>
#include <wiringPi.h>

Q_LOGGING_CATEGORY(CONTROLLER, "ledify.controller", QtWarningMsg)

#define NUM_LEDS        300
#define GPIO_PIN        18
#define DMA             10
#define TARGET_FREQ     WS2811_TARGET_FREQ
#define STRIP_TYPE      SK6812_STRIP_GRBW

LedStripController::LedStripController(QObject *parent) : QObject(parent) {
    connect(this, &LedStripController::terminated, this, &LedStripController::deinitialize);

    m_layerController.addColorLayer(0, 0, 0, 0, 0);
    m_layerController.setAsRootLayer(0);
    m_executor.reset(new CommandExecutor(&m_layerController, &m_fpsCalculator));

    m_loopTimer = new QTimer(this);
    m_loopTimer->setSingleShot(true);
    m_loopTimer->setInterval(5000);
    connect(m_loopTimer, &QTimer::timeout, this, &LedStripController::drawLoop);
}

void LedStripController::turnOnRelayAndRefresh() {
    m_relayController.turnOn();
    if (!m_relayController.isOn()) {
        m_loopTimer->start(c_trafoPowerOnDelayMs);
    } else {
        m_loopTimer->start(0);
    }
}

void LedStripController::initializeDependencies() {
    initializeLedStrip();
    restServer.registerCallback([this] (QString &command) -> QString {
        auto result = parseReceivedString(command);
        turnOnRelayAndRefresh();
        return result;
    });
    connect(this, &LedStripController::drawPixels, this, &LedStripController::drawToLedStrip);
}

void LedStripController::initializeLedStrip() {
    m_ledStrip.freq = TARGET_FREQ;
    m_ledStrip.dmanum = DMA;
    m_ledStrip.channel[0].gpionum = GPIO_PIN;
    m_ledStrip.channel[0].invert = 0;
    m_ledStrip.channel[0].count = NUM_LEDS;
    m_ledStrip.channel[0].strip_type = STRIP_TYPE;
    m_ledStrip.channel[0].brightness = 255;

    ws2811_return_t errCode;
    if ((errCode = ws2811_init(&m_ledStrip)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(errCode));
        emit terminated();
    }
    m_ledBuffer = m_ledStrip.channel[0].leds;
}

void LedStripController::deinitialize() {
    deinitializeLedStrip();
}

void LedStripController::deinitializeLedStrip() {
    ws2811_fini(&m_ledStrip);
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

bool LedStripController::isAnyLedOn() {
    auto rootLayer = m_layerController.rootLayer();
    auto child = rootLayer->child();
    if (!child) {
        return false;
    }

    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (child->pixel(i) != 0) {
             return true;
        }
    }
    return false;
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
    turnOnRelayAndRefresh();
}

void LedStripController::commandOnIfNight() {
    QString str;
    m_executor->cOnIfNight(QStringList(), str);
    turnOnRelayAndRefresh();
}

void LedStripController::startDrawLoop() {
    m_loopTimer->start(0);
}

void LedStripController::terminate() {
    qCDebug(CONTROLLER) << "Terminating...";

    if (!m_relayController.isOn()) {
        emit terminated();
        return;
    }

    connect(&m_relayController, &RelayController::relayStateChanged,
            this, [this] (int, bool state) {
        if (!state) {
            emit terminated();
        }
    });
    commandOff();
    QTimer::singleShot(5000, this, [this] { //force
        m_relayController.turnOff(0);
        emit terminated();
    });
}

void LedStripController::draw() {
    m_fpsCalculator.tick();
    auto *rootLayer = m_layerController.rootLayer();

    rootLayer->startDraw();
    auto child = rootLayer->child();
    if (!child.isNull()) {
        emit drawPixels(child.data());
    }

    rootLayer->endDraw();
}

void LedStripController::drawLoop() {
    auto startMs = millis();
    if (!m_relayController.isOn()) {
        return;
    }

    draw();

    if (animationFinished()) {
        if (isAnyLedOn()) {
            m_loopTimer->start(c_drawRefreshIdleMs);
        } else {
            m_relayController.turnOff(c_trafoIdlePowerOffDelayMs);
        }
    } else {
        auto diffProcessingMs = static_cast<int>(millis() - startMs);
        m_loopTimer->start(qMax(c_drawRefreshAnimationMs - diffProcessingMs, 1));
    }
}

void LedStripController::drawToLedStrip(Layer *rootLayer) {
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        m_ledBuffer[i] = rootLayer->pixel(i);
    }

    ws2811_return_t errCode;
    if ((errCode = ws2811_render(&m_ledStrip)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(errCode));
        terminate();
    }
}

LayerController &LedStripController::layerController() {
    return m_layerController;
}
