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
    auto now = QDateTime::currentDateTime();
    auto isDST = now.timeZone().isDaylightTime(now);
    auto date = now.date();
    auto minsSinceMidForSunrise = m_dusk2dawn->sunrise(date.year(), date.month(), date.day(), isDST);
    auto minsSinceMidForSunset  = m_dusk2dawn->sunset(date.year(), date.month(), date.day(), isDST);

    return minsSinceMidForSunset < minsSinceMidForSunrise;
}
