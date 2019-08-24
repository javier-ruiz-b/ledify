#pragma once

class Dusk2Dawn;
class QDateTime;

class Daytime {
public:
    Daytime();
    ~Daytime();
    bool isDay();
    bool isDay(const QDateTime &dateTime);
    bool isAfterMidnight();
    bool isAfterMidnight(const QDateTime &dateTime);

private:
    Dusk2Dawn *m_dusk2dawn;
};
