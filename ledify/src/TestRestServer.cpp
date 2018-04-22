#include <QCoreApplication>
#include "RestServer.h"

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    RestServer server(&app);
    return app.exec();
}
