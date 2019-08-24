#include "DaytimeTest.h"
#include <Daytime.h>
#include <QDateTime>
#include <QTimeZone>

void DaytimeTest::init() {
    m_tested = new Daytime();
}

void DaytimeTest::cleanup() {
    delete m_tested;
}

void DaytimeTest::isNightAtMidnight() {
    QDateTime dt(QDate(2000, 1, 1), QTime(0, 0));

    QVERIFY(!m_tested->isDay(dt));
}

void DaytimeTest::isDayAtNoon() {
    QDateTime dt(QDate(2000, 1, 1), QTime(12, 0));

    QVERIFY(m_tested->isDay(dt));
}

void DaytimeTest::isAfterMidnight() {
    QDateTime dt(QDate(2000, 1, 1), QTime(1, 0));

    QVERIFY(m_tested->isAfterMidnight(dt));
}

void DaytimeTest::isNotAfterMidnight() {
    QDateTime dt(QDate(2000, 1, 1), QTime(12, 0));

    QVERIFY(!m_tested->isAfterMidnight(dt));
}

QTEST_APPLESS_MAIN(DaytimeTest)
