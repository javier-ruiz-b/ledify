#pragma once

#include <IRelayController.h>

class AlwaysOnRelayController : public IRelayController {
    Q_OBJECT
public:
    explicit AlwaysOnRelayController(QObject *parent = nullptr)
        : IRelayController(parent) {}

    virtual void turnOff(int = 0) {}
    virtual void turnOn(int = 0) {}
    virtual bool isOn() { return true; }

};
