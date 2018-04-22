#pragma once
#include <QObject>
#include "SerialPort.h"
#include "LedStripController.h"

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

private:
    static int sighupFd[2];
    static int sigtermFd[2];

    bool m_running = true;
    QSocketNotifier *snHup;
    QSocketNotifier *snTerm;

    void loop();

    SerialPort serial;
    LedStripController controller;

    void setupUnixSignalHandlers();
    void cleanup();
    bool init();
};

int main (int argc, char **argv);

//bool setup(char *virtSerial);
//void cleanup();
//void loop();

//static bool running = true;


