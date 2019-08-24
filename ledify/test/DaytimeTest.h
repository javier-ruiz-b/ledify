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
    void isAfterMidnight();
    void isNotAfterMidnight();

private:
    Daytime *m_tested;

};
