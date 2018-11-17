#include "RelayController.h"

#include <QTimer>
#include <QObject>
#include "IWiringPi.h"

RelayController::RelayController(IWiringPi *wiringPi, QObject *parent)
    : QObject (parent), m_wiringPi(wiringPi) {
    m_wiringPi->pinMode (c_trafoGpioPin, IWiringPi::output);
    m_wiringPi->digitalWrite (c_trafoGpioPin, IWiringPi::high);
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
        m_wiringPi->digitalWrite (pin, state ? IWiringPi::high : IWiringPi::low);
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
