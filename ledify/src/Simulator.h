#pragma once

#include <QObject>
#include <QScopedPointer>
#include <QVariant>
#include "LedStripController.h"

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
    LedStripController *m_ledStrip;
    QVariant m_ledData;
};
