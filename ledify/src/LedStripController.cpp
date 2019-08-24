#include "LedStripController.h"

#include <cstdio>
#include <QTimer>
#include <QString>
#include <Color.h>
#include <ColorLayer.h>
#include "ILedStrip.h"
#include "Layer.h"
#include "RestClientRelayController.h"
#include "TimeControl.h"

Q_LOGGING_CATEGORY(CONTROLLER, "ledify.controller", QtWarningMsg)

LedStripController::LedStripController(ILedStrip *ledStrip, IRelayController *relayController, QObject *parent)
    : QObject(parent), m_ledStrip(ledStrip), m_relayController(relayController) {
    connect(this, &LedStripController::terminated, this, &LedStripController::deinitialize);

    auto blackColor = m_layerController.add(new ColorLayer(Color(0, 0, 0, 0)));
    m_layerController.setAsRoot(blackColor);
    m_executor.reset(new CommandExecutor(&m_layerController, &m_fpsCalculator));

    m_loopTimer = new QTimer(this);
    m_loopTimer->setSingleShot(true);
    m_loopTimer->setInterval(10*1000);
    connect(m_loopTimer, &QTimer::timeout, this, &LedStripController::drawLoop);
}

void LedStripController::turnOnRelayAndRefresh() {
    m_relayController->turnOn();
    if (!m_relayController->isOn()) {
        m_loopTimer->start(c_trafoPowerOnDelayMs);
    } else {
        m_loopTimer->start(0);
    }
}

void LedStripController::initializeDependencies() {
    m_ledStrip->initialize();
    connect(this, &LedStripController::drawPixels, this, &LedStripController::drawToLedStrip);
}

void LedStripController::deinitialize() {
    m_ledStrip->deinitialize();
}

bool LedStripController::writeChar(char c) {
    if (m_commandReader.receivedChar(c)) {
        return parseReceivedString(m_commandReader.command()).isEmpty();
    }
    return false;
}

bool LedStripController::animationFinished() {
    return m_layerController.root().animationFinished();
}

bool LedStripController::isAnyLedOn() {
    auto rootLayer = m_layerController.root();
    auto child = rootLayer.child();
    if (!child) {
        return false;
    }

    return m_ledStrip->isAnyLedOn();
}

QString LedStripController::parseReceivedString(const QString &received) {
    qCDebug(CONTROLLER) << "Recv:" << received;

    QString response;
    auto lines = received.split(QRegularExpression("(;|\n| )"));
    for (auto line: lines) {
        auto commandAndArgs = line.split("=");
        if (commandAndArgs.empty()) {
            qCDebug(CONTROLLER) << "Empty command";
            return "Empty command";
        }
        auto command = commandAndArgs.first();
        QStringList args;
        if (commandAndArgs.count() >= 2) {
            args = commandAndArgs[1].split(",");
        }

        m_executor->parseCommand(command, args, response);
    }
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

    if (!m_relayController->isOn()) {
        emit terminated();
        return;
    }

    connect(m_relayController, &RestClientRelayController::relayStateChanged,
            this, [this] (bool state) {
        if (!state) {
            emit terminated();
        }
    });
    commandOff();
    QTimer::singleShot(5000, this, [this] { //force
        m_relayController->turnOff(0);
        QTimer::singleShot(1000, this, [this] {
            emit terminated();
        });
    });
}

void LedStripController::draw() {
    m_fpsCalculator.tick();
    auto rootLayer = m_layerController.root();

    auto child = rootLayer.child();
    if (!child.isNull()) {
        emit drawPixels(child.data());
    }
}

void LedStripController::drawLoop() {
    if (!m_relayController->isOn()) {
        return;
    }

    draw();

    if (animationFinished()) {
        if (isAnyLedOn()) {
            m_loopTimer->start(c_drawRefreshIdleMs);
        } else {
            m_relayController->turnOff(c_trafoIdlePowerOffDelayMs);
        }
    } else {
        m_loopTimer->start(0);
    }
}

void LedStripController::drawToLedStrip(Layer *rootLayer) {
    m_ledStrip->render(rootLayer);
}

LayerController &LedStripController::layerController() {
    return m_layerController;
}
