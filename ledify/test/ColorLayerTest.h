#pragma once

#include <QObject>

class ColorLayer;
class ColorLayerTest : public QObject {
    Q_OBJECT

public:
    explicit ColorLayerTest(QObject *parent = nullptr) : QObject(parent) {}

private slots:
    void init();
    void cleanup();

private:
    ColorLayer *m_tested;

};
