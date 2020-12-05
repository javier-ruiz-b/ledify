#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QVariant>
#include <QtQuickControls2>
#include <QScopedPointer>

#include "Simulator.h"
#include "Layer.h"
#include "LedStripController.h"
#include "MockRelayController.h"
#include "Color.h"

#include <thread>

static const int c_numLeds = 300;
static QVector<quint32> m_colorData(c_numLeds);
int main(int argc, char *argv[]) {
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<Simulator>("ledify.simulator", 1, 0, "SimulatorController");

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine(QUrl("qrc:/Simulator.qml"));

    return app.exec();
}

Simulator::Simulator(QObject *parent) : QObject(parent) {
    Config::createInstance(this)->setLedCount(c_numLeds);
    ledStrip.reset(new MockLedStrip());
    m_ledController = new LedStripController(ledStrip.data(), new MockRelayController(this), this);
    connect(m_ledController, &LedStripController::drawPixels, this, [this] (Layer *rootLayer) {
        memset(m_colorData.data(), 0, static_cast<uint32_t>(m_colorData.size()) * sizeof(uint32_t));
        rootLayer->draw(m_colorData);
        for (uint16_t i = 0; i < m_colorData.count(); i++) {
            Color color(static_cast<uint32_t>(m_colorData[i]));
            //simulate white
            int w = color.w()/3;
            Color newColor(qMin(color.r() + w, 255),
                           qMin(color.g() + w, 255),
                           qMin(color.b() + w, 255), 0);
            m_colorData[i] = static_cast<int>(newColor.rgbw());
        }
        setLedData(QVariant::fromValue<QVector<quint32>>(m_colorData));

        constexpr double refreshRateInHz = 120;
        constexpr auto waitDuration = std::chrono::duration<double, std::milli> (1000 / refreshRateInHz);
        std::this_thread::sleep_for(waitDuration);
    });

    m_ledController->startDrawLoop();
    m_ledController->commandOnIfNight();

    QTimer::singleShot(250, [this] {
        this->sendCommand("ON");
    });
}

void Simulator::sendCommand(const QString &string) {
    m_ledController->parseReceivedString(string.toUpper());
    m_ledController->turnOnRelayAndRefresh();
}
