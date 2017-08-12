#pragma once
#include <QObject>

class ColorLayerTest : public QObject {
    Q_OBJECT

public:
    explicit ColorLayerTest(QObject *parent = nullptr) : QObject(parent) {}

private slots:
    void checksPixelColor();

private:

};
