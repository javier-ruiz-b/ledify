#include "RestServer.h"

#include <QLoggingCategory>
#include <QtConcurrent/QtConcurrent>
#include <cpp-httplib/httplib.h>

Q_LOGGING_CATEGORY(REST, "ledify.restserver", QtWarningMsg)

using namespace httplib;


RestServer::RestServer(QObject *parent) : QObject(parent) {
    m_server = new Server();
    m_threadPool = new QThreadPool(this);

    connect(this, &RestServer::executeCommand, this, &RestServer::commandHandler);

    m_threadPool->setMaxThreadCount(1);
    QtConcurrent::run(m_threadPool, [this] {
        startServer();
    });

    qCDebug(REST, "Started REST server");
}

RestServer::~RestServer() {
    m_server->stop();
    delete m_server;
}

QString RestServer::commandHandler(QString command) {
    return m_callback(command);
}

void RestServer::startServer() {
    m_server->Get(R"(.*)", [this](const Request& req, Response& res) {
        QString urlString = req.path.c_str();

       if (urlString.size() < 2) {
           res.set_content("Send me more!\n", "text/plain");
           res.status = 406; //ESTATUS_NOT_ACCEPTABLE
           return;
       }
       auto commands = urlString.mid(1);

       QByteArray responses = "";
       foreach(auto command, commands.split("+", QString::SplitBehavior::SkipEmptyParts)) {
           qCDebug(REST) << "Received command" << command;
           auto response = emit executeCommand(command).toUtf8();
           responses += response + ";";
       }
       res.set_content(responses.data(), "text/plain");

    });

    if (m_server->listen("0.0.0.0", 8033)) {
        qCDebug(REST, "Stopped REST server");
    } else {
        qCWarning(REST, "Error initializing REST server");
    }

}
