#include "RestClientRelayController.h"
#include <QLoggingCategory>
#include <QUrl>
#include <cpp-httplib/httplib.h>
#include <string>

Q_LOGGING_CATEGORY(RESTRELAY, "ledify.restrelay", QtWarningMsg)

using namespace httplib;

RestClientRelayController::RestClientRelayController(QObject *parent)
    : IRelayController(parent) {
    m_timer = new QTimer(this);
}

void RestClientRelayController::turnOff(int delayMs) {
    changeRelayState(false, delayMs);
}

void RestClientRelayController::turnOn(int delayMs) {
    changeRelayState(true, delayMs);
}

bool RestClientRelayController::isOn() {
    return m_state;
}

void RestClientRelayController::changeRelayState(bool state, int delayMs) {
    m_timer->disconnect();
    m_timer->stop();
    connect(m_timer, &QTimer::timeout, this, [this, state] {
        m_timer->stop();
        if (m_state == state) {
            qCDebug(RESTRELAY) << "Same state. Not sending request";
            return;
        } else {
            sendHttpRequest(state);
        }
    });

    m_timer->start(delayMs);
}

void RestClientRelayController::sendHttpRequest(bool originalState) {
    QString ip = "127.0.0.1";
//    QString ip = "192.168.178.50";
    QString protocol = "clarus_switch";
    QString id = "A0";
    QString unit = "0";
    bool inverted = true;

    bool state = inverted ^ originalState;

    QString command = state ? "on" : "off";
    QUrl url("http://" + ip
             + "/send?protocol=" + protocol
             + "&" + command + "=1"
             + "&id=" + id
             + "&unit=" + unit);

    QString request("/send?protocol=" + protocol
             + "&" + command + "=1"
             + "&id=" + id
             + "&unit=" + unit);
    qCDebug(RESTRELAY) << "Sending GET request" << url.toString();

    const int timeoutSecs = 2;
    const int httpPort = 80;
    httplib::Client cli(ip.toUtf8(), httpPort, timeoutSecs);

    auto res = cli.Get(request.toUtf8());
    if (res && res->status == 200) {
        qCDebug(RESTRELAY) << "Response:" << res->body.c_str();
        m_state = originalState;
        emit relayStateChanged(originalState);
        qCDebug(RESTRELAY) << "Response OK, state changed:" << originalState;
    } else {
        qCWarning(RESTRELAY) << "Failed to connect to REST client (pilight)";
    }


}
