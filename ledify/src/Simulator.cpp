#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QVariant>
#include <QtQuickControls2>

#include "Simulator.h"
#include "Layer.h"
#include "LedStripController.h"
#include "MockRelayController.h"
#include "Color.h"

#include <thread>

static const int c_numLeds = SIMULATOR_LEDS;
static QVector<int> m_colorData(c_numLeds);
int main(int argc, char *argv[]) {
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<Simulator>("ledify.simulator", 1, 0, "SimulatorController");

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine(QUrl("qrc:/Simulator.qml"));

    return app.exec();
}

Simulator::Simulator(QObject *parent) : QObject(parent) {
    m_ledController = new LedStripController(&m_ledStrip, new MockRelayController(this), this);
    connect(m_ledController, &LedStripController::drawBuffer, this, [this] (uint32_t *buffer, unsigned long numLeds) {
//        memset(m_colorData.data(), 0, static_cast<uint32_t>(m_colorData.size()) * sizeof(uint32_t));
        memcpy(m_colorData.data(), buffer, static_cast<uint32_t>(numLeds) * sizeof(uint32_t));
//        rootLayer->draw(reinterpret_cast<uint32_t *>(m_colorData.data()),
//                        static_cast<uint32_t>(m_colorData.size()));
        for (uint16_t i = 0; i < m_colorData.count(); i++) {
            Color color(static_cast<uint32_t>(m_colorData[i]));
            //simulate white
            int w = color.w()/3;
            Color newColor(qMin(color.r() + w, 255),
                           qMin(color.g() + w, 255),
                           qMin(color.b() + w, 255), 0);
            m_colorData[i] = static_cast<int>(newColor.rgbw());
        }
        setLedData(QVariant::fromValue<QVector<int>>(m_colorData));

        constexpr double refreshRateInHz = 90;
        constexpr auto waitDuration = std::chrono::duration<double, std::milli> (1000 / refreshRateInHz);
        std::this_thread::sleep_for(waitDuration);
    }, Qt::BlockingQueuedConnection);

    m_ledController->startDrawLoop();
    m_ledController->commandOnIfNight();

    QTimer::singleShot(300, [this] {
        this->sendCommand("ON");
    });
}


//Simulator::Simulator(QObject *parent) : QObject(parent) {
//    m_ledController = new LedStripController(&m_ledStrip, new MockRelayController(this), this);
//    connect(m_ledController, &LedStripController::drawPixels, this, [this] (Layer *rootLayer) {
//        memset(m_colorData.data(), 0, static_cast<uint32_t>(m_colorData.size()) * sizeof(uint32_t));
//        rootLayer->draw(reinterpret_cast<uint32_t *>(m_colorData.data()),
//                        static_cast<uint32_t>(m_colorData.size()));
//        for (uint16_t i = 0; i < m_colorData.count(); i++) {
//            Color color(static_cast<uint32_t>(m_colorData[i]));
//            //simulate white
//            int w = color.w()/3;
//            Color newColor(qMin(color.r() + w, 255),
//                           qMin(color.g() + w, 255),
//                           qMin(color.b() + w, 255), 0);
//            m_colorData[i] = static_cast<int>(newColor.rgbw());
//        }
//        setLedData(QVariant::fromValue<QVector<int>>(m_colorData));

//        constexpr double refreshRateInHz = 120;
//        constexpr auto waitDuration = std::chrono::duration<double, std::milli> (1000 / refreshRateInHz);
//        std::this_thread::sleep_for(waitDuration);
//    });

//    m_ledController->startDrawLoop();
//    m_ledController->commandOnIfNight();

//    QTimer::singleShot(300, [this] {
//        this->sendCommand("ON");
//    });
//}


//Simulator::Simulator(QObject *parent) : QObject(parent) {
//    m_ledController = new LedStripController(&m_ledStrip, new MockRelayController(this), this);
//    QTimer *drawThread = new QTimer(this);
//    connect(drawThread, &QTimer::timeout, this, [this] (Layer *rootLayer) {
//        memset(m_colorData.data(), 0, static_cast<uint32_t>(m_colorData.size()) * sizeof(uint32_t));
//        rootLayer->draw(reinterpret_cast<uint32_t *>(m_colorData.data()),
//                        static_cast<uint32_t>(m_colorData.size()));
//        for (uint16_t i = 0; i < m_colorData.count(); i++) {
//            Color color(static_cast<uint32_t>(m_colorData[i]));
//            //simulate white
//            int w = color.w()/3;
//            Color newColor(qMin(color.r() + w, 255),
//                           qMin(color.g() + w, 255),
//                           qMin(color.b() + w, 255), 0);
//            m_colorData[i] = static_cast<int>(newColor.rgbw());
//        }
//        setLedData(QVariant::fromValue<QVector<int>>(m_colorData));

//        constexpr double refreshRateInHz = 120;
//        constexpr auto waitDuration = std::chrono::duration<double, std::milli> (1000 / refreshRateInHz);
//        std::this_thread::sleep_for(waitDuration);
//    });
//    drawThread->setInterval(5);
//    drawThread->start();

//    m_ledController->startDrawLoop();
//    m_ledController->commandOnIfNight();

//    QTimer::singleShot(300, [this] {
//        this->sendCommand("ON");
//    });
//}


void Simulator::sendCommand(const QString &string) {
    m_ledController->parseReceivedString(string.toUpper());
    m_ledController->turnOnRelayAndRefresh();
}
