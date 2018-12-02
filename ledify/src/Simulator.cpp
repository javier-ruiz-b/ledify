#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QVariant>
#include <QtQuickControls2>

#include "Simulator.h"
#include "Layer.h"
#include "LedStripController.h"
#include "Color.h"

static const int c_numLeds = 300;
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
    m_ledController = new LedStripController(&m_ledStrip, &m_wiringPi, this);
    connect(m_ledController, &LedStripController::drawPixels, this, [this] (Layer *rootLayer) {
        rootLayer->draw(reinterpret_cast<uint32_t *>(m_colorData.data()),
                        static_cast<uint32_t>(m_colorData.size()));
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
    });

    m_ledController->startDrawLoop();
    m_ledController->commandOnIfNight();
}

void Simulator::sendCommand(const QString &string) {
    m_ledController->parseReceivedString(string);
    m_ledController->turnOnRelayAndRefresh();
}
