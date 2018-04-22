#include "RestServer.h"

#include <qhttpserver.hpp>
#include <qhttpserverrequest.hpp>
#include <qhttpserverresponse.hpp>

using namespace qhttp::server;

RestServer::RestServer(QObject *parent) : QObject(parent) {
    m_server = new QHttpServer(this);
    m_server->listen(QHostAddress::Any, 8033, [this] (QHttpRequest* req, QHttpResponse* res) {
        emit requestReceived(req, res);
    });

    if ( m_server->isListening() ) {
        qDebug("Started REST server");
    } else {
        qWarning("Failed to start REST server");
    }
}

void RestServer::requestReceived(QHttpRequest *req, QHttpResponse *res) {
    QString urlString = req->url().toString();

    if (urlString.size() < 2) {
        res->setStatusCode(qhttp::ESTATUS_NOT_ACCEPTABLE);
        res->end("Send me more!\n");
        return;
    }
    auto command = urlString.mid(1);
    res->setStatusCode(qhttp::ESTATUS_OK);
    res->end("OK!\n");
    qDebug() << "Received command" << command;

    emit receivedCommand(command);
}
