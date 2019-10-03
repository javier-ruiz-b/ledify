#pragma once
#include <QObject>
#include <QTimer>
#include "IRelayController.h"

class RestClientRelayController : public IRelayController {
    Q_OBJECT
public:
    explicit RestClientRelayController(QObject *parent = nullptr);

    virtual void turnOff(int delayMs = 2000);
    virtual void turnOn(int delayMs = 0);
    virtual bool isOn();

private:
    void changeRelayState(bool state, int delayMs);
    void sendHttpRequest(bool state);

private:
    QTimer *m_timer;
    bool m_state = false;
};
