#pragma once
#include <QObject>
#include "SerialPort.h"
#include "LedStripController.h"

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
    static int sighupFd[2];
    static int sigtermFd[2];

    QSocketNotifier *snHup;
    QSocketNotifier *snTerm;

    SerialPort serial;
    LedStripController *controller;

};

int main (int argc, char **argv);


