#pragma once
#include <QObject>
#include <QTimer>
#include <IRelayController.h>

class MockRelayController : public IRelayController {
    Q_OBJECT
public:
    explicit MockRelayController(QObject *parent = nullptr);

    virtual void turnOff(int delayMs = 2000);
    virtual void turnOn(int delayMs = 0);
    virtual bool isOn();

private:
    void changeState(bool state, int delayMs);

private:
    QTimer *m_timer;
    bool m_state = false;
};
