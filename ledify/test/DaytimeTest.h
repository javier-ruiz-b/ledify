#pragma once
#include <QTest>

class Daytime;

class DaytimeTest : public QObject {
    Q_OBJECT

public:
    DaytimeTest() {}

private slots:
    void init();
    void cleanup();

    void isNightAtMidnight();
    void isDayAtNoon();

private:
    Daytime *m_tested;

};
