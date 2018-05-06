#pragma once
#include <QObject>
#include <QString>
#include <functional>

namespace qhttp {
namespace server {
class QHttpServer;
class QHttpRequest;
class QHttpResponse;
}
}
using namespace qhttp::server;

class RestServer : public QObject {
    Q_OBJECT

public:
    RestServer(QObject *parent = nullptr);
    void registerCallback(std::function<QString(QString &)> callback) { m_callback = callback; }

private slots:
    void requestReceived(QHttpRequest *req, QHttpResponse* res);

private:
    QHttpServer *m_server = nullptr;
    std::function<QString(QString &)> m_callback = [](QString &){return QString(); };

};
