#include "Daytime.h"
#include <QDateTime>
#include <QTimeZone>
#include <Dusk2Dawn.h>

Daytime::Daytime() {
    float berlinLat = 52.520008f;
    float berlinLon = 13.404954f;
    auto now = QDateTime::currentDateTime();
    float offsetFromUtcHours = now.timeZone().offsetFromUtc(now) / 3600.0f;
    m_dusk2dawn = new Dusk2Dawn(berlinLat, berlinLon, offsetFromUtcHours);
}

Daytime::~Daytime() {
    delete m_dusk2dawn;
}

bool Daytime::isDay() {
    return isDay(QDateTime::currentDateTime());
}

bool Daytime::isDay(const QDateTime &dateTime) {
    auto isDST = dateTime.timeZone().isDaylightTime(dateTime);
    auto date = dateTime.date();
    auto minsSinceMidForSunrise = m_dusk2dawn->sunrise(date.year(), date.month(), date.day(), isDST);
    auto minsSinceMidForSunset  = m_dusk2dawn->sunset(date.year(), date.month(), date.day(), isDST);

    auto time = dateTime.time();
    auto currentMinutes = time.hour() * 60 + time.minute();

    return minsSinceMidForSunrise < currentMinutes &&
            currentMinutes < minsSinceMidForSunset;
}

bool Daytime::isAfterMidnight() {
    return isAfterMidnight(QDateTime::currentDateTime());
}

bool Daytime::isAfterMidnight(const QDateTime &dateTime) {
    if (isDay(dateTime)) {
        return false;
    }

    auto hour = dateTime.time().hour();
    return (0 <= hour) && (hour <= 12);
}
