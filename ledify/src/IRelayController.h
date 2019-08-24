#pragma once

#include <QObject>

class IRelayController : public QObject {
    Q_OBJECT
public:
    explicit IRelayController(QObject *parent = nullptr)
        : QObject(parent) {}

    virtual void turnOff(int delayMs = 2000) = 0;
    virtual void turnOn(int delayMs = 0) = 0;
    virtual bool isOn() = 0;

signals:
    void relayStateChanged(bool state);

};
