#pragma once
#include <QObject>
#include <QString>

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

private slots:
    void requestReceived(QHttpRequest *req, QHttpResponse* res);

signals:
    void receivedCommand(const QString &string);

private:
    QHttpServer *m_server = nullptr;

};
