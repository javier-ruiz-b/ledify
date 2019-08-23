#pragma once
#include <QObject>
#include <QTimer>

namespace qhttp {
namespace client {
class QHttpClient;
class QHttpRequest;
class QHttpResponse;
}
}
using namespace qhttp::client;

class RestClientRelayController : public QObject {
    Q_OBJECT
public:
    explicit RestClientRelayController(QObject *parent = nullptr);

    void turnOff(int delayMs = 2000);
    void turnOn(int delayMs = 0);
    bool isOn();

signals:
    void relayStateChanged(bool state);

private:
    void changeRelayState(bool state, int delayMs);
    void sendHttpRequest(bool state);

private:
    QHttpClient *m_httpClient;
    QTimer *m_timer;
    bool m_state = false;
};
