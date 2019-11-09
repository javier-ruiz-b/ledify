#pragma once

#include <QtTest>
class LedStripController;

class SmokeTest : public QObject {
    Q_OBJECT
public:
    explicit SmokeTest(QObject *parent = nullptr);


public slots:

private slots:
    void init();
    void cleanup();
    void readTestFiles();

    bool runTestFile(QString &contents);
};

