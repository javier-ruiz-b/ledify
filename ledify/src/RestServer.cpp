#include "RestServer.h"

#include <qhttpserver.hpp>
#include <qhttpserverrequest.hpp>
#include <qhttpserverresponse.hpp>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(REST, "ledify.restserver", QtWarningMsg)

using namespace qhttp::server;

RestServer::RestServer(QObject *parent) : QObject(parent) {
    m_server = new QHttpServer(this);
    m_server->listen(QHostAddress::Any, 8033, [this] (QHttpRequest* req, QHttpResponse* res) {
        emit requestReceived(req, res);
    });

    if ( m_server->isListening() ) {
        qCDebug(REST, "Started REST server");
    } else {
        qCWarning(REST, "Failed to start REST server");
    }
}

void RestServer::requestReceived(QHttpRequest *req, QHttpResponse *res) {
    QString urlString = req->url().toString();

    if (urlString.size() < 2) {
        res->setStatusCode(qhttp::ESTATUS_NOT_ACCEPTABLE);
        res->end("Send me more!\n");
        return;
    }
    auto commands = urlString.mid(1);

    QByteArray responses = "";
    foreach(auto command, commands.split("+", QString::SplitBehavior::SkipEmptyParts)) {
        qCDebug(REST) << "Received command" << command;
        responses += m_callback(command).toUtf8() + ";";
    }
    res->setStatusCode(qhttp::ESTATUS_OK);
    res->end(responses);
}
