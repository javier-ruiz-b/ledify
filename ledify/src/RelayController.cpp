#include "RelayController.h"

#include <QTimer>
#include <QObject>
#include <wiringPi.h>

RelayController::RelayController(QObject *parent)
    : QObject (parent) {
    pinMode (c_trafoGpioPin, OUTPUT);
    digitalWrite (c_trafoGpioPin, HIGH);
}

void RelayController::turnOff(int delayMs) {
    changeRelayState(c_trafoGpioPin, false, delayMs);
}

void RelayController::turnOn(int delayMs) {
    changeRelayState(c_trafoGpioPin, true, delayMs);
}

bool RelayController::isOn() {
    return m_pinState[c_trafoGpioPin];
}

void RelayController::changeRelayState(int pin, bool state, int delayMs) {
    auto timer = getTimer(pin);
    connect(timer, &QTimer::timeout, this, [this, pin, state, timer] {
        digitalWrite (pin, state ? HIGH : LOW);
        m_pinState[c_trafoGpioPin] = state;
        timer->disconnect();
        emit relayStateChanged(pin, state);
    });
    timer->start(delayMs);
}


QTimer *RelayController::getTimer(int pin) {
    auto timer = m_pinDelayTimers[pin];
    if (timer == nullptr) {
        timer = new QTimer(this);
        timer->setSingleShot(true);
        m_pinDelayTimers[pin] = timer;
    }
    timer->disconnect();
    return timer;
}
