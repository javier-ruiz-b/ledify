#include "ledify.h"
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <ws2811.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#include "LayerController.h"
#include <QtDebug>
#include <QTimer>

#define NUM_LEDS        300
#define GPIO_PIN        18
#define DMA             10
#define TARGET_FREQ     WS2811_TARGET_FREQ
#define STRIP_TYPE      SK6812_STRIP_GRBW
#define TERMINATE_MS    1500
#define FPS             35

int Ledify::sighupFd[2];
int Ledify::sigtermFd[2];

ws2811_t ledStrip {
    .render_wait_time = 0,
    .device = nullptr,
    .rpi_hw = nullptr,
    .freq = TARGET_FREQ,
    .dmanum = DMA,
    .channel =
    {
        {
            .gpionum = GPIO_PIN,
            .invert = 0,
            .count = NUM_LEDS,
            .strip_type = STRIP_TYPE,
            .leds = nullptr,
            .brightness = 255,
            .wshift = 0,
            .rshift = 0,
            .gshift = 0,
            .bshift = 0,
            .gamma = 0
        }
    },
};

int main(int argc, char **argv) {
    QCoreApplication a(argc, argv);

    auto *ledify = new Ledify(a.arguments(), &a);
    QObject::connect(ledify, &Ledify::finished, &a, &QCoreApplication::quit);
    QTimer::singleShot(0, ledify, &Ledify::run);
    return a.exec();
}

void Ledify::terminate() {
    qDebug() << "Terminating...";

    LayerController &layerControl = controller.layerController();
    layerControl.addColorLayer(0, 60, 40, 5, 100);
    layerControl.addColorLayer(1, 0, 0, 0, 0);
    layerControl.addFadeLayer(2, 0, 1, 0, FadeLayer::InterpolatorDecelerate, TERMINATE_MS - 100);
    layerControl.setAsRootLayer(2);
    QTimer::singleShot(TERMINATE_MS, this, [this] {
           m_running = false;
       });
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

bool Ledify::init() {
    if (wiringPiSetup() == -1) {
        qCritical() << "Unable to start wiringPi:"
                    << strerror (errno);
        return false;
    }

    ws2811_return_t errCode;
    if ((errCode = ws2811_init(&ledStrip)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(errCode));
        return false;
    }
    return true;
}

void Ledify::cleanup() {
    ws2811_fini(&ledStrip);
}

void Ledify::run() {
    if (!init()) {
        emit finished();
        return;
    }

    LayerController &layerControl = controller.layerController();
    layerControl.addColorLayer(0, 0, 0, 0, 0);
    layerControl.addColorLayer(1, 60, 40, 5, 100);
    layerControl.addFadeLayer(2, 0, 1, 0, FadeLayer::InterpolatorAccelerate, 5000);
    layerControl.setAsRootLayer(2);

    QTimer::singleShot(0, this, &Ledify::loop);
}

void Ledify::loop() {
//    if (serial.available()) {
//        char c = serial.read();
//        qDebug().noquote().nospace() <<"%c";
//        controller.writeChar(c);
//    }
//    uint32_t leds[300];
//    controller.draw(static_cast<uint32_t *>(leds), NUM_LEDS);
    controller.draw(static_cast<uint32_t *>(ledStrip.channel[0].leds), NUM_LEDS);

    ws2811_return_t errCode;
    if ((errCode = ws2811_render(&ledStrip)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(errCode));
        m_running = false;
    }

    if (!m_running) {
        cleanup();
        qDebug() << "Finished";
        emit finished();
        return;
    }
    QTimer::singleShot(1000 / FPS, this, &Ledify::loop);
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
