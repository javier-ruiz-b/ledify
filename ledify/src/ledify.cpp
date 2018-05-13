#include "ledify.h"
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "LayerController.h"
#include <QtDebug>
#include <QString>

int Ledify::sighupFd[2];
int Ledify::sigtermFd[2];

int main(int argc, char **argv) {
    QCoreApplication a(argc, argv);

    auto *ledify = new Ledify(a.arguments(), &a);
    QObject::connect(ledify, &Ledify::finished, &a, &QCoreApplication::quit);
    QTimer::singleShot(0, ledify, &Ledify::run);
    return a.exec();
}

Ledify::Ledify(const QStringList &arguments, QObject *parent) : QObject(parent) {
    setupUnixSignalHandlers();

    QString serialInput = "/dev/ttyAMA0";
    if (arguments.size() >= 2) {
        if (arguments[0] == "-D") {
            serialInput = arguments[1];
        }
    }
    serial.begin(serialInput.toUtf8().constData(), 9600);
}

void Ledify::setupUnixSignalHandlers() {
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sighupFd))
       qFatal("Couldn't create HUP socketpair");
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigtermFd))
       qFatal("Couldn't create TERM socketpair");
    snHup = new QSocketNotifier(sighupFd[1], QSocketNotifier::Read, this);
    connect(snHup, SIGNAL(activated(int)), this, SLOT(handleSigHup()));
    snTerm = new QSocketNotifier(sigtermFd[1], QSocketNotifier::Read, this);
    connect(snTerm, SIGNAL(activated(int)), this, SLOT(handleSigTerm()));
    setupStaticUnixSignalHandlers();
}

bool Ledify::init() {
    controller = new LedStripController();
    connect (controller, &LedStripController::terminated, this, &Ledify::finished);
    controller->initialize();
    return true;
}

void Ledify::run() {
    if (!init()) {
        emit finished();
        return;
    }
    controller->startDrawLoop();
    controller->commandOnIfNight();
}

void Ledify::terminate() {
    controller->terminate();
}

int Ledify::setupStaticUnixSignalHandlers() {
    struct sigaction hup, term;

    hup.sa_handler = Ledify::hupSignalHandler;
    sigemptyset(&hup.sa_mask);
    hup.sa_flags = 0;
    hup.sa_flags |= SA_RESTART;

    if (sigaction(SIGHUP, &hup, 0))
       return 1;

    term.sa_handler = Ledify::termSignalHandler;
    sigemptyset(&term.sa_mask);
    term.sa_flags |= SA_RESTART;

    if (sigaction(SIGTERM, &term, 0))
       return 2;

    return 0;
}

void Ledify::hupSignalHandler(int) {
    char a = 1;
    ::write(sighupFd[0], &a, sizeof(a));
}

void Ledify::termSignalHandler(int) {
    char a = 1;
    ::write(sigtermFd[0], &a, sizeof(a));
}

void Ledify::handleSigHup() {
    snHup->setEnabled(false);
    char tmp;
    ::read(sighupFd[1], &tmp, sizeof(tmp));

    terminate();

    snHup->setEnabled(true);
}

void Ledify::handleSigTerm() {
    snTerm->setEnabled(false);
    char tmp;
    ::read(sigtermFd[1], &tmp, sizeof(tmp));

    terminate();

    snTerm->setEnabled(true);
}
