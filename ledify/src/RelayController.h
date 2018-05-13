#pragma once

#include <QMap>
#include <QObject>

class QTimer;

class RelayController: public QObject {
    Q_OBJECT

public:
    RelayController(QObject *parent = nullptr);

    void turnOff(int delayMs = 2000);
    void turnOn(int delayMs = 0);
    bool isOn();

signals:
    void relayStateChanged(int pin, bool state);

private:
    void changeRelayState(int pin, bool state, int delayMs);
    QTimer* getTimer(int pin);

private:
    const int c_trafoGpioPin = 29;
    QMap<int, QTimer *> m_pinDelayTimers;
    QMap<int, bool> m_pinState;

};
