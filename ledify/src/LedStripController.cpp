#include "LedStripController.h"

#include "Layer.h"
#include <cstdio>
#include <QTimer>
#include <QString>
#include <ws2811.h>
#include <wiringPi.h>

Q_LOGGING_CATEGORY(CONTROLLER, "ledify.controller", QtWarningMsg)

#define NUM_LEDS        300
#define GPIO_PIN        18
#define DMA             10
#define TARGET_FREQ     WS2811_TARGET_FREQ
#define STRIP_TYPE      SK6812_STRIP_GRBW
#define FPS             40

ws2811_t ledStrip {
    .render_wait_time = 0,
    .device = nullptr,
    .rpi_hw = nullptr,
    .freq = TARGET_FREQ,
    .dmanum = DMA,
    .channel =
    {
        {
            .gpionum = GPIO_PIN,
            .invert = 0,
            .count = NUM_LEDS,
            .strip_type = STRIP_TYPE,
            .leds = nullptr,
            .brightness = 255,
            .wshift = 0,
            .rshift = 0,
            .gshift = 0,
            .bshift = 0,
            .gamma = 0
        }
    },
};

LedStripController::LedStripController() {
    connect(this, &LedStripController::terminated, this, &LedStripController::deinitialize);

    m_layerController.addColorLayer(0, 0, 0, 0, 0);
    m_layerController.setAsRootLayer(0);
    m_executor.reset(new CommandExecutor(&m_layerController, &m_fpsCalculator));

    m_loopTimer = new QTimer(this);
    m_loopTimer->setSingleShot(true);
    m_loopTimer->setInterval(5000);
    connect(m_loopTimer, &QTimer::timeout, this, &LedStripController::drawLoop);
}

void LedStripController::initialize() {
    initializeWiringPi();
    initializeLedStrip();
    restServer.registerCallback([this] (QString &command) -> QString {
        auto result = parseReceivedString(command);
        m_relayController.turnOn();
        if (!m_relayController.isOn()) {
            m_loopTimer->start(2000);
        } else {
            m_loopTimer->start(0);
        }
        return result;
    });
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

bool LedStripController::isAnyLedOn() {
    for (int i = 0; i < NUM_LEDS; i++) {
        if (ledStrip.channel[0].leds[i] != 0) {
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
}

void LedStripController::commandOnIfNight() {
    QString str;
    m_executor->cOnIfNight(QStringList(), str);
}

void LedStripController::startDrawLoop() {
    m_loopTimer->start(0);
}

void LedStripController::terminate() {
    qCDebug(CONTROLLER) << "Terminating...";

    connect(&m_relayController, &RelayController::relayStateChanged,
            this, [this] (int, bool state) {
        if (!state) {
            emit terminated();
        }
    });
    commandOff();
    QTimer::singleShot(3000, this, [this] { //force
        m_relayController.turnOff(0);
        emit terminated();
    });
    //    m_terminating = true;
}

void LedStripController::initializeWiringPi() {
    if (wiringPiSetup() == -1) {
        qCritical(CONTROLLER) << "Unable to start wiringPi:"
                    << strerror (errno);
        emit terminated();
    }
}

void LedStripController::initializeLedStrip() {
    ws2811_return_t errCode;
    if ((errCode = ws2811_init(&ledStrip)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(errCode));
        emit terminated();
    }

}

void LedStripController::deinitialize() {
    deinitializeLedStrip();
}

void LedStripController::deinitializeLedStrip() {
    ws2811_fini(&ledStrip);
}

void LedStripController::drawLoop() {
    auto startMs = millis();
    if (!m_relayController.isOn()) {
        return;
    }

    draw(static_cast<uint32_t *>(ledStrip.channel[0].leds), NUM_LEDS);

    ws2811_return_t errCode;
    if ((errCode = ws2811_render(&ledStrip)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(errCode));
        terminate();
    }

    if (animationFinished()) {
        if (isAnyLedOn()) {
            m_loopTimer->start(5000);
        } else {
            m_relayController.turnOff();
        }
    } else {
        const auto sleepMs = 1000 / FPS;
        auto diffProcessingMs = static_cast<int>(millis() - startMs);
        m_loopTimer->start(qMax(sleepMs - diffProcessingMs, 1));
    }
}

LayerController &LedStripController::layerController() {
    return m_layerController;
}
