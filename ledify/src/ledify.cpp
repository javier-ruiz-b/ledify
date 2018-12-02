#include "ledify.h"
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <wiringPi.h>

#include "LayerController.h"
#include <QtDebug>
#include <QString>

int Ledify::s_sighupFd[2];
int Ledify::s_sigtermFd[2];

int main(int argc, char **argv) {
    QCoreApplication a(argc, argv);

    auto *ledify = new Ledify(a.arguments(), &a);
    QObject::connect(ledify, &Ledify::finished, &a, &QCoreApplication::quit);
    QTimer::singleShot(0, ledify, &Ledify::run);
    return a.exec();
}

Ledify::Ledify(const QStringList &arguments, QObject *parent)
    : QObject(parent), m_ledStrip(300) {
    setupUnixSignalHandlers();

    QString serialInput = "/dev/ttyAMA0";
    if (arguments.size() >= 2) {
        if (arguments[0] == "-D") {
            serialInput = arguments[1];
        }
    }
    m_serial.begin(serialInput.toUtf8().constData(), 9600);
}

void Ledify::setupUnixSignalHandlers() {
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, s_sighupFd))
       qFatal("Couldn't create HUP socketpair");
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, s_sigtermFd))
       qFatal("Couldn't create TERM socketpair");
    m_snHup = new QSocketNotifier(s_sighupFd[1], QSocketNotifier::Read, this);
    connect(m_snHup, SIGNAL(activated(int)), this, SLOT(handleSigHup()));
    m_snTerm = new QSocketNotifier(s_sigtermFd[1], QSocketNotifier::Read, this);
    connect(m_snTerm, SIGNAL(activated(int)), this, SLOT(handleSigTerm()));
    setupStaticUnixSignalHandlers();
}

bool Ledify::init() {
    if (wiringPiSetup() == -1) {
        qCritical() << "Unable to start wiringPi:"
                    << strerror (errno);
        emit finished();
        return false;
    }
    auto controller = new LedStripController(&m_ledStrip, &m_wiringPi, this);
    connect (controller, &LedStripController::terminated, this, &Ledify::finished);
    controller->initializeDependencies();
    restServer.registerCallback([controller] (QString &command) -> QString {
        auto result = controller->parseReceivedString(command);
        controller->turnOnRelayAndRefresh();
        return result;
    });
    m_controller = controller;
    return true;
}

void Ledify::run() {
    if (!init()) {
        emit finished();
        return;
    }
    m_controller->startDrawLoop();
    m_controller->commandOnIfNight();
}

void Ledify::terminate() {
    m_controller->terminate();
}

int Ledify::setupStaticUnixSignalHandlers() {
    struct sigaction hup, term;

    hup.sa_handler = Ledify::hupSignalHandler;
    sigemptyset(&hup.sa_mask);
    hup.sa_flags = 0;
    hup.sa_flags |= SA_RESTART;

    if (sigaction(SIGHUP, &hup, nullptr))
       return 1;

    term.sa_handler = Ledify::termSignalHandler;
    sigemptyset(&term.sa_mask);
    term.sa_flags |= SA_RESTART;

    if (sigaction(SIGTERM, &term, nullptr))
       return 2;

    return 0;
}

void Ledify::hupSignalHandler(int) {
    char a = 1;
    ::write(s_sighupFd[0], &a, sizeof(a));
}

void Ledify::termSignalHandler(int) {
    char a = 1;
    ::write(s_sigtermFd[0], &a, sizeof(a));
}

void Ledify::handleSigHup() {
    m_snHup->setEnabled(false);
    char tmp;
    ::read(s_sighupFd[1], &tmp, sizeof(tmp));

    terminate();

    m_snHup->setEnabled(true);
}

void Ledify::handleSigTerm() {
    m_snTerm->setEnabled(false);
    char tmp;
    ::read(s_sigtermFd[1], &tmp, sizeof(tmp));

    terminate();

    m_snTerm->setEnabled(true);
}
