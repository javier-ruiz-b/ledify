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
#include "VideoBuffer.h"

Q_LOGGING_CATEGORY(CONTROLLER, "ledify.controller", QtWarningMsg)

LedStripController::LedStripController(ILedStrip *ledStrip, IRelayController *relayController, QObject *parent)
    : QObject(parent), m_ledStrip(ledStrip), m_relayController(relayController) {
    connect(this, &LedStripController::terminated, this, &LedStripController::deinitialize);

    auto blackColor = m_layerController.add(new ColorLayer(Color(0, 0, 0, 0)));
    m_layerController.setAsRoot(blackColor);
    m_executor.reset(new CommandExecutor(&m_layerController, &m_fpsCalculator));

    m_videoBuffer = new VideoBuffer(ledStrip->numLeds());
    m_videoBuffer->registerRenderCallback([this] (uint32_t *buffer, size_t numLeds) -> bool {
        if (!m_relayController->isOn()) {
            return false;
        }
        auto child = m_layerController.root().child();
        if (!child.isNull()) {
            m_fpsCalculator.tick();
            child->draw(buffer, static_cast<uint32_t>(numLeds));
//                m_ledStrip->draw(buffer);
        }


//        if (isAnyLedOn()) {
////            m_loopTimer->start(c_drawRefreshIdleMs);

//            auto child = m_layerController.root().child();
//            if (!child.isNull()) {
//                m_fpsCalculator.tick();
//                child->draw(buffer, static_cast<uint32_t>(numLeds));
////                m_ledStrip->draw(buffer);
//            }
//        } else {
//            m_relayController->turnOff(c_trafoIdlePowerOffDelayMs);
//            return false;
//        }
        return true;
    });
    m_videoBuffer->registerDrawerCallback([this] (uint32_t *buffer, size_t size) -> bool {
        emit drawBuffer(buffer, size);
        return true;
    });

    connect(this, &LedStripController::drawBuffer, this, [this] (uint32_t *buffer, unsigned long) {
        m_ledStrip->draw(buffer);

        auto isOn = m_ledStrip->isAnyLedOn();
        if (isOn != m_wasAnyLedOn) {
            if (isOn && !m_relayController->isOn()) {
                m_relayController->turnOn();
            } else if (!isOn && m_relayController->isOn()) {
                m_relayController->turnOff(c_trafoIdlePowerOffDelayMs);
            }
            m_wasAnyLedOn = isOn;
        }
        return true;
    }, Qt::BlockingQueuedConnection);

    m_ledStrip->initialize();
    m_loopTimer = new QTimer(this);
//    m_loopTimer->setSingleShot(true);
//    m_loopTimer->setInterval(10*1000);
//    connect(m_loopTimer, &QTimer::timeout, this, &LedStripController::drawLoop);
//    connect(this, &LedStripController::drawPixels, this, &LedStripController::drawToLedStrip);
}

void LedStripController::turnOnRelayAndRefresh() {
    m_relayController->turnOn();
    if (!m_relayController->isOn()) {
        m_loopTimer->start(c_trafoPowerOnDelayMs);
    } else {
        m_loopTimer->start(0);
    }
}

void LedStripController::deinitialize() {
    m_videoBuffer->stop();
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
    return m_executor->parseCommand(received);
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
    m_videoBuffer->start();
}

void LedStripController::terminate() {
    qCDebug(CONTROLLER) << "Terminating...";

    if (!m_relayController->isOn()) {
        emit terminated();
        return;
    }

    connect(m_relayController, &RestClientRelayController::relayStateChanged,
            this, [this] (bool isOn) {
        if (!isOn) {
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
    uint32_t* buffer = new uint32_t[m_videoBuffer->numLeds()];
    rootLayer->draw(buffer, m_videoBuffer->numLeds());
    m_ledStrip->draw(buffer);

    delete[] buffer;
}

LayerController &LedStripController::layerController() {
    return m_layerController;
}
