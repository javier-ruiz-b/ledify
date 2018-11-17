#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QVariant>
#include <QtQuickControls2>

#include "Simulator.h"
#include "Layer.h"
#include "LedStripController.h"

static const int c_numLeds = 64;
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
    m_ledController = new LedStripController(&m_ledStrip, c_numLeds, &m_wiringPi, this);
    connect(m_ledController, &LedStripController::drawPixels, this, [this] (Layer *rootLayer) {
        for (uint16_t i = 0; i < m_colorData.count(); i++) {
            m_colorData[i] = static_cast<int>(rootLayer->pixel(i));
        }
        setLedData(QVariant::fromValue<QVector<int>>(m_colorData));
    });

    m_ledController->startDrawLoop();
    m_ledController->commandOnIfNight();
}

void Simulator::sendCommand(const QString &string) {
    m_ledController->parseReceivedString(string);
    m_ledController->turnOnRelayAndRefresh();
}
