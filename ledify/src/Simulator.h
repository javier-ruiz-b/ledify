#pragma once

#include <QObject>
#include <QScopedPointer>
#include <QVariant>

#include "MockLedStrip.h"
#define SIMULATOR_LEDS 300

class LedStripController;

class Simulator : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariant ledData READ ledData WRITE setLedData NOTIFY onLedDataChanged)

public:
    Simulator(QObject *parent = nullptr);

    QVariant ledData() const {
        return m_ledData;
    }

signals:
    void drawReady();
    void onLedDataChanged(const QVariant &ledData);

public slots:
    Q_INVOKABLE void sendCommand(const QString &string);

    void setLedData(QVariant ledData) {
        m_ledData = ledData;
        emit onLedDataChanged(ledData);
    }

private:
    LedStripController *m_ledController;
    MockLedStrip m_ledStrip{SIMULATOR_LEDS};
    QVariant m_ledData;
};
