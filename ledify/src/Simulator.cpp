#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QVariant>
#include <QtQuickControls2>

#include "Simulator.h"
#include "Layer.h"

static QVector<int> m_colorData(64);

int main(int argc, char *argv[]) {
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<Simulator>("ledify.simulator", 1, 0, "SimulatorController");

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine(QUrl("qrc:/Simulator.qml"));

    return app.exec();
}

Simulator::Simulator(QObject *parent) : QObject(parent) {
    m_ledStrip = new LedStripController(this);
    connect(m_ledStrip, &LedStripController::drawPixels, this, [parent, this] (Layer *rootLayer) {
        for (uint16_t i = 0; i < m_colorData.count(); i++) {
            m_colorData[i] = static_cast<int>(rootLayer->pixel(i));
        }
        setLedData(QVariant::fromValue<QVector<int>>(m_colorData));
    });

    m_ledStrip->startDrawLoop();
    m_ledStrip->commandOnIfNight();
}

void Simulator::sendCommand(const QString &string) {
    m_ledStrip->parseReceivedString(string);
    m_ledStrip->turnOnRelayAndRefresh();
}
