#include "RestClientRelayController.h"
#include <qhttpclient.hpp>
#include <qhttpclientresponse.hpp>
#include <qhttpfwd.hpp>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(RESTRELAY, "ledify.restrelay", QtDebugMsg)

using namespace qhttp::client;

RestClientRelayController::RestClientRelayController(QObject *parent) : QObject(parent) {
    m_timer = new QTimer(this);
    m_httpClient = new QHttpClient(this);
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

    qCDebug(RESTRELAY) << "Sending GET request" << url.toString();

    m_httpClient->killConnection();
    m_httpClient = new QHttpClient(this);
    m_httpClient->setConnectingTimeOut(2000, [] {
        qCWarning(RESTRELAY, "Failed to connect to REST client (pilight)");
    });
    m_httpClient->request(qhttp::EHTTP_GET, url, [this, originalState](QHttpResponse* res) {
        res->onData([](const QByteArray& chunk) {
            qCDebug(RESTRELAY).noquote() << "Data:" << chunk;
        });

        if (res->status() != qhttp::ESTATUS_OK) {
            qCWarning(RESTRELAY) << "Response code" << res->status() << res->statusString();
        } else {
            m_state = originalState;
            emit relayStateChanged(originalState);
            qCDebug(RESTRELAY) << "Response OK, state changed:" << originalState;
        }
    });



}
