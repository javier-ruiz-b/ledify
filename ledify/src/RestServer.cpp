#include "RestServer.h"

#include <QLoggingCategory>
#include <cpp-httplib/httplib.h>

Q_LOGGING_CATEGORY(REST, "ledify.restserver", QtWarningMsg)

using namespace httplib;

RestServer::RestServer(QObject *parent) : QObject(parent) {

    Server svr;

    svr.Get(R"(.*)", [this](const Request& req, Response& res) {
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
           responses += m_callback(command).toUtf8() + ";";
       }
       res.set_content(responses.data(), "text/plain");

    });


    if (svr.listen("0.0.0.0", 8033)) {
        qCDebug(REST, "Started REST server");
    } else {
        qCWarning(REST, "Failed to start REST server");
    }
}
