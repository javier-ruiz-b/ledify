#include "MockRelayController.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(MOCKRELAY, "ledify.mockrelay", QtDebugMsg)


MockRelayController::MockRelayController(QObject *parent)
    : IRelayController(parent) {
    m_timer = new QTimer(this);
}

void MockRelayController::turnOff(int delayMs) {
    changeState(false, delayMs);
}

void MockRelayController::turnOn(int delayMs) {
    changeState(true, delayMs);
}

bool MockRelayController::isOn() {
    return m_state;
}

void MockRelayController::changeState(bool state, int delayMs) {
    m_timer->disconnect();
    m_timer->stop();

    connect(m_timer, &QTimer::timeout, this, [this, state] {
        m_timer->stop();
        if (m_state == state) {
            return;
        } else {
            m_state = state;
            emit relayStateChanged(state);
            if (state) {
                qCDebug(MOCKRELAY) << "Relay turned on";
            } else {
                qCDebug(MOCKRELAY) << "Relay turned off";
            }
        }
    });

    m_timer->start(delayMs);
}
