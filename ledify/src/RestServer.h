#pragma once
#include <QObject>
#include <QString>
#include <QThreadPool>
#include <functional>

namespace httplib {
class Server;
}

class RestServer : public QObject {
    Q_OBJECT

public:
    RestServer(QObject *parent = nullptr);
    ~RestServer();

    void registerCallback(std::function<QString(QString &)> callback) { m_callback = callback; }

signals:
    QString executeCommand(QString command);

public slots:
    QString commandHandler(QString command);


private:
    void startServer();

    std::function<QString(QString &)> m_callback = [](QString &){return QString(); };
    httplib::Server *m_server;
    QThreadPool *m_threadPool;

};
