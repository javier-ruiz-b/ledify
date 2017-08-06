#pragma once

#include <QObject>

class ColorConversionTest : public QObject {
    Q_OBJECT

public:
    explicit ColorConversionTest(QObject *parent = nullptr) : QObject(parent) {}

private slots:
    void init();
    void cleanup();

private:

};
