#pragma once
#include <QObject>
#include "SerialPort.h"
#include "LedStripController.h"
#include "Ws2811LedStrip.h"
#include "RestServer.h"

#define NUM_LEDS 300

class QTimer;

class Ledify : public QObject {
    Q_OBJECT

public:
    Ledify(const QStringList &arguments, QObject *parent = nullptr);

    // Unix signal handlers.
    static void hupSignalHandler(int unused);
    static void termSignalHandler(int unused);

    static int setupStaticUnixSignalHandlers();

    void run();
    void terminate();

signals:
    void finished();

public slots:
    // Qt signal handlers.
    void handleSigHup();
    void handleSigTerm();
    void setupUnixSignalHandlers();
    bool init();

private:
    static int s_sighupFd[2];
    static int s_sigtermFd[2];

    QSocketNotifier *m_snHup;
    QSocketNotifier *m_snTerm;

    SerialPort m_serial;
    LedStripController *m_controller;

    Ws2811LedStrip m_ledStrip;

    RestServer restServer;
};

int main (int argc, char **argv);


